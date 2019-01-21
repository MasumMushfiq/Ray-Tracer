#include <bits/stdc++.h>
#include "vector_3d.h"
#include "point.h"
#include "sphere.h"
#include "pyramid.h"
#include "light_source.h"
#include "spotlight.h"
#include "bitmap_image.hpp"

#include <GL/glut.h>

using namespace std;

using pdc = pair<double, color>;

const double EPSILON = 1.0e-7;
const int CHECKER_BOARD_EXPANSION = 5;

int draw_axes;

double near, far, fov_x, fov_y, aspect_ratio;
int level_of_recursion;
unsigned screen_size;
float cb_width;
double ambient_cb, diffuse_cb, reflection_cb;
double no_of_objects;
double no_of_light_sources;
double no_of_spotlights;
double far_t, top_far_t;
bool texture_mode;
color black = {0, 0, 0};
color white = {1, 1, 1};

vector<sphere> spheres;
vector<pyramid> pyramids;
vector<light_source> light_sources;
vector<spotlight> spotlights;
vector<vector<point>> point_buffer;
vector<vector<color>> pixels;
vector<vector<color>> texture;

struct camera {
    point pos;
    vector_3d u, r, l;

    const int CAMERA_POSITION_MOVEMENT = 5; // unit co-ordinate change
    const int CAMERA_ANGLE_MOVEMENT = 5;   // degree to rotate

    camera() {
        init_camera();
    }

    void init_camera() {
        pos = point(20, -150, 50);

        l = vector_3d(0, 1, 0);
        u = vector_3d(0, 0, 1);
        r = vector_3d(1, 0, 0);
    }

    void move_forward() { pos = pos + (l * CAMERA_POSITION_MOVEMENT); }

    void move_backward() { pos = pos + (l * (-1) * CAMERA_POSITION_MOVEMENT); }

    void move_right() { pos = pos + (r * CAMERA_POSITION_MOVEMENT); }

    void move_left() { pos = pos + (r * (-1) * CAMERA_POSITION_MOVEMENT); }

    void move_up() { pos = pos + (u * CAMERA_POSITION_MOVEMENT); }

    void move_down() { pos = pos + (u * (-1) * CAMERA_POSITION_MOVEMENT); }

    void look_left() {
        r = r.rotate_3d(CAMERA_ANGLE_MOVEMENT, u);
        l = l.rotate_3d(CAMERA_ANGLE_MOVEMENT, u);
    }

    void look_right() {
        r = r.rotate_3d(-CAMERA_ANGLE_MOVEMENT, u);
        l = l.rotate_3d(-CAMERA_ANGLE_MOVEMENT, u);
    }

    void look_up() {
        u = u.rotate_3d(CAMERA_ANGLE_MOVEMENT, r);
        l = l.rotate_3d(CAMERA_ANGLE_MOVEMENT, r);
    }

    void look_down() {
        u = u.rotate_3d(-CAMERA_ANGLE_MOVEMENT, r);
        l = l.rotate_3d(-CAMERA_ANGLE_MOVEMENT, r);
    }

    void tilt_clockwise() {
        u = u.rotate_3d(-CAMERA_ANGLE_MOVEMENT, l);
        r = r.rotate_3d(-CAMERA_ANGLE_MOVEMENT, l);
    }

    void tilt_counterclockwise() {
        u = u.rotate_3d(CAMERA_ANGLE_MOVEMENT, l);
        r = r.rotate_3d(CAMERA_ANGLE_MOVEMENT, l);
    }

    void reset() { init_camera(); }

} the_camera;

struct result {
    double t;
    color c;
    vector_3d normal;
    vector_3d reflected_ray;
    point intersection;

    result(double t, color c, vector_3d n, vector_3d rr, point i) :
            t(t), c(c), normal(n), reflected_ray(rr), intersection(i) {
    }

    result() {
        t = -1;
        c = {0, 0, 0};
        normal = {0, 0, 0};
        reflected_ray = vector_3d(0, 0, 0);
        intersection = point(0, 0, 0);
    }

    point get_advanced_origin() {
        return intersection + reflected_ray * 0.0001;
    }
};

struct light_properties {
    double ambient{}, diffuse{}, specular{}, reflection{};
    double shininess{};

    light_properties(double a, double d, double s, double r, double sh) :
            ambient(a), diffuse(d), specular(s), reflection(r), shininess(sh) {}
};

inline bool is_equal(double d1, double d2) { return abs(d1 - d2) <= EPSILON; }

void drawAxes() {
    if (draw_axes == 1) {
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f(1000, 0, 0);
            glVertex3f(-1000, 0, 0);

            glVertex3f(0, -1000, 0);
            glVertex3f(0, 1000, 0);

            glVertex3f(0, 0, 1000);
            glVertex3f(0, 0, -1000);
        }
        glEnd();
    }
}

void drawSphere(double radius, int slices, int stacks) {
    struct point points[100][100];
    int i, j;
    double h, r;
    //generate points
    for (i = 0; i <= stacks; i++) {
        h = radius * sin(((double) i / (double) stacks) * (PI / 2));
        r = radius * cos(((double) i / (double) stacks) * (PI / 2));
        for (j = 0; j <= slices; j++) {
            points[i][j].x = r * cos(((double) j / (double) slices) * 2 * PI);
            points[i][j].y = r * sin(((double) j / (double) slices) * 2 * PI);
            points[i][j].z = h;
        }
    }
    //draw quads using generated points
    for (i = 0; i < stacks; i++) {
        for (j = 0; j < slices; j++) {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                //lower hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3d(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3d(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawPyramid(double width, double height) {
    glBegin(GL_TRIANGLES);
    {
        glVertex3d(0, 0, 0);
        glVertex3d(0, width, 0);
        glVertex3d(width / 2.0, width / 2.0, height);

        glVertex3d(0, width, 0);
        glVertex3d(width, width, 0);
        glVertex3d(width / 2.0, width / 2.0, height);

        glVertex3d(width, width, 0);
        glVertex3d(width, 0, 0);
        glVertex3d(width / 2.0, width / 2.0, height);

        glVertex3d(width, 0, 0);
        glVertex3d(0, 0, 0);
        glVertex3d(width / 2.0, width / 2.0, height);

        glVertex3d(0, 0, 0);
        glVertex3d(0, width, 0);
        glVertex3d(width, width, 0);

        glVertex3d(0, 0, 0);
        glVertex3d(width, width, 0);
        glVertex3d(width, 0, 0);
    }
    glEnd();
}

void drawCheckerBoard() {
    auto no_of_boards = static_cast<int>(far * CHECKER_BOARD_EXPANSION / cb_width);
    int limit = 2 * no_of_boards;

    float base_x = -cb_width * no_of_boards;
    float base_y = cb_width * no_of_boards;

    for (int i = 0; i < limit; ++i) {
        float y = base_y - i * cb_width;

        for (int j = 0; j < limit; ++j) {
            float x = base_x + j * cb_width;

            float a = cb_width;
            float clr = (i + j) % 2;
            glColor3f(clr, clr, clr);
            glBegin(GL_QUADS);
            {
                glVertex3f(x, y, 0);
                glVertex3f(x + a, y, 0);
                glVertex3f(x + a, y - a, 0);
                glVertex3f(x, y - a, 0);
            }
            glEnd();
        }
    }
}

color get_color_from_checkerboard(point intersection) {
    double nb = floor(far * CHECKER_BOARD_EXPANSION / cb_width);
    auto i = static_cast<int>(floor((intersection.x + cb_width * nb) / cb_width));
    auto j = static_cast<int>(floor((cb_width * nb - intersection.y) / cb_width));
    int clr = (i + j) % 2;
    float c = clr * 1.0f;
    if (texture_mode and clr) {
        double baseX = -cb_width * nb;
        double baseY = cb_width * nb;
        double delX = intersection.x - baseX;
        double delY = baseY - intersection.y;

        double x = delX - (i * cb_width);
        double y = delY - (j * cb_width);
        auto height = static_cast<unsigned int>(texture[0].size());
        auto width = static_cast<unsigned int>(texture.size());
        auto col = static_cast<int>(round(y / cb_width * height));
        auto row = static_cast<int>(round(x / cb_width * width));
        if (row >= width)
            row--;
        if (col >= height)
            col--;
        return texture[row][col];
    }
    return {c, c, c};
}

void generate_point_buffer() {
    point_buffer = vector<vector<point>>(static_cast<unsigned long>(screen_size), vector<point>(
            static_cast<unsigned long>(screen_size)));

    double half_y = near * tan(fov_y / 2 * (PI / 180.0));
    double half_x = near * tan(fov_x / 2 * (PI / 180.0));
    point center = the_camera.pos + the_camera.l * near;
    point center_top = center + the_camera.u * half_y;
    point top_left = center_top + the_camera.r * (-1 * half_x);

    double del_y = half_y / screen_size;
    double del_x = half_x / screen_size;
    for (int i = 0; i < screen_size; ++i) {
        point base_row = top_left + the_camera.u * (-1) * (2 * i + 1) * del_y;

        for (int j = 0; j < screen_size; ++j) {
            point p = base_row + the_camera.r * (2 * j + 1) * del_x;
            point_buffer[i][j] = p;
        }
    }

    cout << "Point buffer generation done" << endl;
}

bool is_beyond_far(double t) {
    return t >= far_t;
}

// ray must be normalized
result intersect_checker_board(const vector_3d &ray, const point &ray_origin) {
    // returns t and color at intersecting point
    assert(is_equal(ray.length(), 1.0));

    result default_result;

    vector_3d normal;
    normal = ray_origin.z < 0 ? vector_3d(0, 0, -1) : vector_3d(0, 0, 1);

    if (normal.dot(ray) == 0) {
        return default_result; // ray is parallel to checkerboard; so no intersection point
    }
    point on_the_plane(0, 0, 0);
    double t = (on_the_plane - ray_origin).dot(normal) / (ray.dot(normal));
    if (t < 0) {
        return default_result; // intersects behind the eye
    }
    if (is_beyond_far(t)) {  // roughly clipping in the far region
        return default_result;
    }

    point intersection = ray_origin + ray * t;

    assert(is_equal(intersection.z, 0.0));

    color c = get_color_from_checkerboard(intersection);
    vector_3d reflected_ray = ray.reflect(normal);
    result r(t, c, normal, reflected_ray, intersection);
    return r;
}

// ray must be normalized
result intersect_sphere(const vector_3d &ray, const point &ray_origin, const sphere &s) {
    assert(is_equal(ray.length(), 1.0));

    result default_result;

    double a = 1;
    double b = 2 * ray.dot(ray_origin - s.center);
    double c = (ray_origin - s.center).dot(ray_origin - s.center) - (s.radius * s.radius);

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return default_result;
    } else if (is_equal(discriminant, 0.0)) {
        double t = -b / (2 * a);
        if (t < 0 or is_beyond_far(t)) {
            // behind the eye or further than far plane
            return default_result;
        }
        point intersection = ray_origin + ray * t;
        vector_3d normal = s.get_normal_at_point(intersection);
        vector_3d rr = ray.reflect(normal);
        result r(t, s.colour, normal, rr, intersection);
        return r;
    } else {
        double t0 = (-b + sqrt(discriminant)) / (2 * a);
        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        if (t0 > t1) {
            swap(t0, t1);
        }
        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) {
                // both are behind the eye
                return default_result;
            }
        }
        if (is_beyond_far(t0)) {
            return default_result;
        }
        point intersection = ray_origin + ray * t0;
        vector_3d normal = s.get_normal_at_point(intersection);
        vector_3d rr = ray.reflect(normal);
        result r(t0, s.colour, normal, rr, intersection);
        return r;
    }
}

// ray must be normalized
result intersect_triangle(const vector_3d &ray, const point &origin,
                          const point &v0, const point &v1, const point &v2) {

    assert(is_equal(ray.length(), 1.0));
    result default_result;

    // edge vectors
    vector_3d e1 = v1 - v0;
    vector_3d e2 = v2 - v0;

    // face normal
    vector_3d n = e1.cross(e2).normalize();

    vector_3d q = ray.cross(e2);
    double a = e1.dot(q);

    // nearly parallel ??
    if (is_equal(a, 0)) {
        return default_result;
    }

    vector_3d s = (origin - v0) * (1.0 / a);
    vector_3d r = s.cross(e1);

    double b0 = s.dot(q);
    double b1 = r.dot(ray);
    double b2 = 1.0 - b0 - b1;

    // intersected outside triangle ??
    if (b0 < 0 or b1 < 0 or b2 < 0) {
        return default_result;
    }

    double t = e2.dot(r);
    if (t < 0 or is_beyond_far(t)) {
        // outside viewing planes
        return default_result;
    }
    point intersection = origin + ray * t;
    vector_3d rr = ray.reflect(n);
    return {t, white, n, rr, intersection};
}

// ray must be normalized
result intersect_pyramid(const vector_3d &ray, const point &ray_origin, const pyramid &p) {
    assert(is_equal(ray.length(), 1.0));
    result default_result;

    vector<result> ts;
    ts.push_back(intersect_triangle(ray, ray_origin, p.top, p.lower_right, p.lower_left));
    ts.push_back(intersect_triangle(ray, ray_origin, p.top, p.lower_left, p.upper_left));
    ts.push_back(intersect_triangle(ray, ray_origin, p.top, p.upper_left, p.upper_right));
    ts.push_back(intersect_triangle(ray, ray_origin, p.top, p.upper_right, p.lower_right));
    ts.push_back(intersect_triangle(ray, ray_origin, p.upper_right, p.lower_right, p.upper_left));
    ts.push_back(intersect_triangle(ray, ray_origin, p.lower_right, p.upper_left, p.lower_left));

    sort(begin(ts), end(ts), [](result r1, result r2) -> bool { return r1.t < r2.t; });

    if (is_equal(ts[5].t, -1.0)) {
        return default_result;
    }

    for (auto t : ts) {
        if (t.t > 0.0) {
            t.c = p.colour;
            return t;
        }
    }
    return default_result;
}

bool is_illuminates_ls(const light_source &ls, const point &intersection) {
    vector_3d ray = ls.position - intersection;
    double distance = ray.length();
    ray = ray.normalize();
    point ray_origin = intersection + ray * 0.000001;

    auto res = intersect_checker_board(ray, ray_origin);
    if (res.t > 0 and res.t < distance) {
        return false;
    }
    for (const auto &s : spheres) {
        res = intersect_sphere(ray, ray_origin, s);
        if (res.t > 0 and res.t < distance) {
            return false;
        }
    }

    for (const auto &p : pyramids) {
        res = intersect_pyramid(ray, ray_origin, p);
        if (res.t > 0 and res.t < distance) {
            return false;
        }
    }
    return true;
}

bool is_illuminates_sp(const spotlight &sp, const point &intersection) {
    if (!is_illuminates_ls(sp, intersection)) {
        return false;
    }
    vector_3d v1 = (intersection - sp.position).normalize();
    vector_3d v2 = sp.direction;
    double angle = acos(v1.dot(v2)) * (180.0 / PI);

    return angle <= sp.cutoff_angle;
}

pair<double, double> get_lambert_and_phong(const point &intersection, const vector_3d &normal,
                                           const vector_3d &reflected_ray, double shininess) {
    double lambert = 0, phong = 0;
    for (const auto &ls : light_sources) {
        if (!is_illuminates_ls(ls, intersection)) {
            continue;
        }
        vector_3d to_source = ls.position - intersection;
        double distance = to_source.length();
        to_source = to_source.normalize();
        vector_3d N = normal.normalize();
        double scaling_factor = exp(-distance * distance * ls.fall_off);
        lambert += to_source.dot(N) * scaling_factor;
        vector_3d rr = reflected_ray.normalize();
        phong += pow(rr.dot(N), shininess) * scaling_factor;
    }

    for (const auto &sp : spotlights) {
        if (!is_illuminates_sp(sp, intersection)) {
            continue;
        }
        vector_3d to_source = sp.position - intersection;
        double distance = to_source.length();
        to_source = to_source.normalize();
        vector_3d N = normal.normalize();
        double scaling_factor = exp(-distance * distance * sp.fall_off);
        lambert += to_source.dot(N) * scaling_factor;
        vector_3d rr = reflected_ray.normalize();
        phong += pow(rr.dot(N), shininess) * scaling_factor;
    }


    return {lambert, phong};
}

color get_color_for_ray(const vector_3d &ray, const point &ray_origin, int level) {
    if (level == 0) {
        return black;
    }

    result pixel;

    light_properties lp(ambient_cb, diffuse_cb, 0, reflection_cb, 0);

    pixel = intersect_checker_board(ray, ray_origin);

    for (const auto &s : spheres) {
        auto x = intersect_sphere(ray, ray_origin, s);
        if (pixel.t < 0 and x.t > 0) {
            pixel = x;
            lp = {s.ambient, s.diffuse, s.specular, s.reflection, s.shininess};
        } else if (x.t < pixel.t and x.t != -1) {
            pixel = x;
            lp = {s.ambient, s.diffuse, s.specular, s.reflection, s.shininess};
        }
    }

    for (const auto &p : pyramids) {
        auto x = intersect_pyramid(ray, ray_origin, p);
        if (pixel.t < 0 and x.t > 0) {
            pixel = x;
            lp = {p.ambient, p.diffuse, p.specular, p.reflection, p.shininess};
        } else if (x.t < pixel.t and x.t != -1) {
            pixel = x;
            lp = {p.ambient, p.diffuse, p.specular, p.reflection, p.shininess};
        }
    }

    pair<double, double> lambert_phong = {0, 0};
    double lambert = 0.0, phong = 0.0;

    far_t = top_far_t - pixel.t;
    color reflected_color = black;
    if (pixel.t > 0.0) {
        lambert_phong = get_lambert_and_phong(pixel.intersection, pixel.normal,
                                              pixel.reflected_ray, lp.shininess);
        lambert = lambert_phong.first;
        phong = lambert_phong.second;
        reflected_color = get_color_for_ray(pixel.reflected_ray,
                                            pixel.get_advanced_origin(), level - 1);
    }

    auto c = pixel.c * (lp.ambient + lp.diffuse * lambert + lp.specular * phong) +
             (reflected_color * lp.reflection);

    return c;
}

void save_image() {
    bitmap_image image(screen_size, screen_size);
    for (unsigned y = 0; y < screen_size; y++) {
        for (unsigned x = 0; x < screen_size; x++) {
            image.set_pixel(x, y, static_cast<const unsigned char>(pixels[y][x].r * 255),
                            static_cast<const unsigned char>(pixels[y][x].g * 255),
                            static_cast<const unsigned char>(pixels[y][x].b * 255));
        }
    }
    image.save_image("out.bmp");
}

void print_status(int row) {
    for (int i = 1; i <= 10; ++i) {
        if (row == (screen_size / 10 * i) - 1) {
            cout << "Rendering " << i * 10 << "% complete" << endl;
        }
    }
}

void trace_rays() {
    generate_point_buffer();

    if (texture_mode) {
        cout << "Rendering image with texture" << endl;
    }
    pixels = vector<vector<color>>(static_cast<unsigned long>(screen_size), vector<color>(
            static_cast<unsigned long>(screen_size)));
    for (int i = 0; i < point_buffer.size(); ++i) {
        for (int j = 0; j < point_buffer[i].size(); ++j) {
            point ray_origin = point_buffer[i][j];
            vector_3d ray = ray_origin - the_camera.pos;
            ray = ray.normalize();
            top_far_t = far / (ray.dot(the_camera.l));
            far_t = top_far_t;

            pixels[i][j] = get_color_for_ray(ray, ray_origin, level_of_recursion);
        }
        print_status(i);
    }

    save_image();
    cout << "Image Saved" << endl;
}

void load_texture() {
    bitmap_image texture_image("texture.bmp");
    unsigned height = texture_image.height();
    unsigned width = texture_image.width();
    texture = vector<vector<color>>(width, vector<color>(height));
    for (unsigned i = 0; i < width; i++) {
        for (unsigned j = 0; j < height; j++) {
            unsigned char r, g, b;
            texture_image.get_pixel(i, j, r, g, b);
            color c(r / 255.0f, g / 255.0f, b / 255.0f);
            texture[i][j] = c;
        }
    }
    cout << "Texture load complete" << endl;
}

void keyboardListener(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            the_camera.look_left();
            break;
        case '2':
            the_camera.look_right();
            break;
        case '3':
            the_camera.look_up();
            break;
        case '4':
            the_camera.look_down();
            break;
        case '5':
            the_camera.tilt_clockwise();
            break;
        case '6':
            the_camera.tilt_counterclockwise();
            break;
        case '0':
            trace_rays();
            break;
        case ' ':
            texture_mode = !texture_mode;
            break;
        default:
            break;
    }
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN: //down arrow key
            the_camera.move_backward();
            break;
        case GLUT_KEY_UP: // up arrow key
            the_camera.move_forward();
            break;

        case GLUT_KEY_RIGHT:
            the_camera.move_right();
            break;
        case GLUT_KEY_LEFT:
            the_camera.move_left();
            break;

        case GLUT_KEY_PAGE_UP:
            the_camera.move_up();
            break;
        case GLUT_KEY_PAGE_DOWN:
            the_camera.move_down();
            break;

        case GLUT_KEY_INSERT:
            break;

        case GLUT_KEY_HOME:
            the_camera.reset();
            break;
        case GLUT_KEY_END:
            break;
        default:
            break;
    }
}

void mouseListener(int button, int state, int x, int y) { //x, y is the x-y of the screen (2D
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) { // 2 times?? in ONE click? -- solution is checking DOWN or UP
                //draw_axes = 1 - draw_axes;

            }
            break;

        case GLUT_RIGHT_BUTTON:
            //........
            if (state == GLUT_DOWN) { // 2 times?? in ONE click? -- solution is checking DOWN or UP
                //draw_grid = 1 - draw_grid;

            }
            break;

        case GLUT_MIDDLE_BUTTON:
            //........
            break;

        default:
            break;
    }
}

void display() {
    {
        //clear the display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0); //color black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************
	    / set-up camera here
	    ********************/
        //load the correct matrix -- MODEL-VIEW matrix
        glMatrixMode(GL_MODELVIEW);

        //initialize the matrix
        glLoadIdentity();

        //now give three info
        //1. where is the camera (viewer)?
        //2. where is the camera looking?
        //3. Which direction is the camera's UP direction?

        //gluLookAt(50, 50, -100, 0, 0, 0, 1, 0, 0);
        //gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
        //gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
        point p = the_camera.pos.add_vector(the_camera.l);
        gluLookAt(the_camera.pos.x, the_camera.pos.y, the_camera.pos.z,
                  p.x, p.y, p.z, the_camera.u.i, the_camera.u.j, the_camera.u.k);

        //again select MODEL-VIEW
        glMatrixMode(GL_MODELVIEW);
    }
    /****************************
	/ Add your objects from here
	****************************/
    //add objects

    drawAxes();

    drawCheckerBoard();

    for (const auto &s : spheres) {
        glPushMatrix();
        glColor3f(s.colour.r, s.colour.g, s.colour.b);
        glTranslated(s.center.x, s.center.y, s.center.z);
        drawSphere(s.radius, 32, 32);
        glPopMatrix();
    }

    for (const auto &p : pyramids) {
        glPushMatrix();
        glColor3f(p.colour.r, p.colour.g, p.colour.b);
        glTranslated(p.lower_left.x, p.lower_left.y, p.lower_left.z);
        drawPyramid(p.width, p.height);
        glPopMatrix();
    }

    for (const auto &l : light_sources) {
        glPushMatrix();
        glColor3f(white.r, white.g, white.b);
        glTranslated(l.position.x, l.position.y, l.position.z);
        drawSphere(5, 32, 32);
        glPopMatrix();
    }

    for (const auto &spotlight : spotlights) {
        glPushMatrix();
        glColor3f(white.r, white.g, white.b);
        glTranslated(spotlight.position.x, spotlight.position.y, spotlight.position.z);
        drawSphere(5, 32, 32);
        glPopMatrix();
    }

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate() {

    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init() {
    //codes for initialization
    draw_axes = 0;
    texture_mode = false;

    load_texture();

    //clear the screen
    glClearColor(0, 0, 0, 0);
//    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    /************************
	/ set-up projection here
	************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
//    gluPerspective(80, 1, 1, 1500.0);
    gluPerspective(fov_y, aspect_ratio, near, far);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

void take_input() {
    ifstream description;
    description.open("description.txt");
    if (!description.is_open()) {
        cerr << "Cannot find description file" << endl;
        exit(EXIT_FAILURE);
    }

    description >> near >> far >> fov_y >> aspect_ratio;
    description >> level_of_recursion >> screen_size;
    description >> cb_width;
    description >> ambient_cb >> diffuse_cb >> reflection_cb;
    description >> no_of_objects;

    fov_x = fov_y * aspect_ratio;

    assert(ambient_cb + diffuse_cb + reflection_cb == 1.0);

    for (int i = 0; i < no_of_objects; ++i) {
        string type;
        description >> type;
        if (type == "sphere") {
            sphere sp;
            description >> sp;
            spheres.push_back(sp);
        } else if (type == "pyramid") {
            pyramid py;
            description >> py;
            py.complete_pyramid();
            pyramids.push_back(py);
        } else {
            cerr << "Unknown object\n";
            exit(EXIT_FAILURE);
        }
    }

    description >> no_of_light_sources;

    for (int j = 0; j < no_of_light_sources; ++j) {
        point p;
        double fall_off;
        description >> p >> fall_off;
        light_sources.emplace_back(p, fall_off);
    }

    description >> no_of_spotlights;

    for (int k = 0; k < no_of_spotlights; ++k) {
        point pos, looking_at;
        double fall_off, angle;

        description >> pos >> fall_off;
        description >> looking_at >> angle;

        spotlights.emplace_back(pos, fall_off, looking_at, angle);
    }


    cout << "Done taking input" << endl;
}

int main(int argc, char **argv) {
    take_input();

    glutInit(&argc, argv);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //Depth, Double buffer, RGB color

    glutCreateWindow("RAY");

    init();

    glEnable(GL_DEPTH_TEST); //enable Depth Testing

    glutDisplayFunc(display); //display callback function
    glutIdleFunc(animate);    //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop(); //The main loop of OpenGL

    return 0;
}
