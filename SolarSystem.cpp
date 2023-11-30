#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define PI 3.14

// Biến lưu trữ tọa độ chuột khi di chuyển
int lastMouseX, lastMouseY;
// Góc xoay theo trục X và Y
float rotateX = 0.0;
float rotateY = 0.0;
// Biến kiểm tra việc nhấn chuột
bool mouseDown = false;
// Biến để thay đổi tỷ lệ phóng to/thu nhỏ
float zoom = 1.0;

// Góc quay của các hành tinh và vật thể
float angleMoon = 0.0, angleEarth = 0.0, angleAstroid = 0.0,
angleMars = 0.0,
angleMercury = 0.0,
angleVenus = 0.0,
angleJupiter = 0.0,
angleSaturn = 0.0,
angleUranus = 30.0,
angleNeptune = 60.0;

// Tỉ lệ kích thước của hành tinh và vật thể
GLfloat sx = 0.2, sy = 0.2, sz = 0.2;

// Mảng màu sắc
GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat blue[] = { 0.0f, 0.0f, 0.9f, 1.0f };
GLfloat er[] = { 0.0f, 5.0f, 0.9f, 1.0f };
GLfloat yellow[] = { 0.7f, 0.2f, 0.0f, 1.0f };
GLfloat qAmb[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat qDif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat qSpec[] = { .50, .50, .50, .10 };
GLfloat qPos[] = { 0, 0, 0, 0.1 };
GLfloat sc[8] = { 0.295, 0.40, 0.50, 0.60, 0.80, 1.0, 1.05, 1.13 };
double ang = 2 * PI / 300;
double angular = 2 * PI / 50;

// Khởi tạo ánh sáng
void initLighting()
{
    // Bật chế độ ánh sáng, cho phép ánh sáng ảnh hưởng đến đối tượng trong không gian 3D.
    glEnable(GL_LIGHTING);

    // Bật nguồn ánh sáng cụ thể, ở đây là GL_LIGHT7.
    glEnable(GL_LIGHT7);

    // Thiết lập màu ánh sáng môi trường (ambient light) cho GL_LIGHT7.
    // Ánh sáng ambient tỏa đều mọi nơi và không có hướng.
    glLightfv(GL_LIGHT7, GL_AMBIENT, qAmb);

    // Thiết lập màu ánh sáng khuếch tán (diffuse light) cho GL_LIGHT7.
    // Ánh sáng diffuse là ánh sáng phản xạ từ bề mặt và phân tán theo góc pháp tuyến.
    glLightfv(GL_LIGHT7, GL_DIFFUSE, qDif);

    // Thiết lập màu ánh sáng phản xạ (specular light) cho GL_LIGHT7.
    // Ánh sáng specular là ánh sáng phản xạ từ bề mặt theo hướng nghịch của hướng nhìn.
    glLightfv(GL_LIGHT7, GL_SPECULAR, qSpec);
}

// Khởi tạo cấu hình ban đầu
void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPointSize(1.0);
    glLineWidth(2.0);
}

// Vẽ quỹ đạo
void orbit()
{
    glColor3f(0.5, 0.5, 0.5);

    int i = 0;
    for (i = 0; i < 8; i++)
    {
        glPushMatrix();
        // Góc quay của quỹ đạo (đặc biệt cho hành tinh thứ 5 và thứ 6)
        if (i == 5)
        {
            glRotatef(45, 1.0, 0.0, 0.0);
        }
        else
        {
            glRotatef(63, 1.0, 0.0, 0.0);
        }
        glScalef(sc[i], sc[i], sc[i]);

        glBegin(GL_POINTS);
        double ang1 = 0.0;
        int i = 0;
        // Vẽ các điểm trên quỹ đạo
        for (i = 0; i < 300; i++)
        {
            glVertex2d(cos(ang1), sin(ang1));
            ang1 += ang;
        }
        glEnd();

        glPopMatrix();
    }
}

void drawSphereSun(float R) {
    glBegin(GL_POINTS);
    const double Delta_U = 0.1;
    const double Delta_V = 0.1;
    for (double v = -M_PI / 2; v < M_PI / 2; v += Delta_V) {
        for (double u = 0; u < 2 * M_PI; u += Delta_U) {
            float x = R * cos(u) * cos(v);
            float y = R * sin(u) * cos(v);
            float z = R * sin(v);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void drawSphere(float R) {
    glBegin(GL_POLYGON);
    const double Delta_U = 0.1;
    const double Delta_V = 0.1;
    for (double v = -M_PI / 2; v < M_PI / 2; v += Delta_V) {
        for (double u = 0; u < 2 * M_PI; u += Delta_U) {

            float x = R * cos(u) * cos(v);
            float y = R * sin(u) * cos(v);
            float z = R * sin(v);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}


// Hàm vẽ các hành tinh và vật thể
void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    // Áp dụng phóng to/thu nhỏ
    glScalef(zoom, zoom, zoom);

    orbit();

    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();

    glColor3f(0.7, 0.5, 0.0);
    glScalef(sx, sy, sz);
    glLightfv(GL_LIGHT7, GL_POSITION, qPos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
    drawSphere(1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    glPopMatrix();

    // Xoay theo các giá trị được điều khiển bởi chuột
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    // Áp dụng phóng to/thu nhỏ
    glScalef(zoom, zoom, zoom);

    // Vẽ các hành tinh
    glScalef(0.2, 0.2, 0.2);

    // Hành tinh Mercury
    glPushMatrix();
    glRotatef(angleMercury, 0.0, 1.0, -0.5);
    glTranslatef(1.5, 0.0, 0.0);
    glColor3f(1.0, 0.9, 0.0);
    glScalef(0.08, 0.08, 0.08);
    drawSphere(1);
    glPopMatrix();

    // Hành tinh Venus
    glPushMatrix();
    glRotatef(angleVenus, 0.0, 1.0, -0.5);
    glTranslatef(2.0, 0.0, 0.0);
    glColor3f(0.9, 0.1, 0.0);
    glScalef(0.1, 0.1, 0.1);
    drawSphere(1);
    glPopMatrix();

    // Hành tinh Earth
    glPushMatrix();
    glRotatef(angleEarth, 0.0, 1.0, -0.5);
    glTranslatef(2.5, 0.0, 0.0);
    glColor3f(0.0, 0.1, 0.7);
    glScalef(0.23, 0.23, 0.23);
    drawSphere(1);

    // Mặt Trăng quay quanh Trái Đất
    glPushMatrix();
    glRotatef(angleMoon, 0.0, 0.1, 0.05);
    glTranslatef(1.3, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(0.5);
    glPopMatrix(); // Mặt Trăng được tạo ra
    glPopMatrix(); // Trái Đất được tạo ra

    // Hành tinh Mars
    glPushMatrix();
    glRotatef(angleMars, 0.0, 1.0, -0.5);
    glTranslatef(-3.0, 0.0, 0.0);
    glColor3f(0.05, 0.05, 0.01);
    glScalef(0.17, 0.17, 0.17);
    drawSphere(1);
    glPopMatrix();

    // Quỹ đạo của hành tinh Mars
    glPushMatrix();
    glColor3f(3.30, 3.30, 3.30);
    glRotatef(63, 1.0, 0.0, 0.0);
    int j = 0, i = 0, div = 90;
    float siz = 2;
    float scl[4] = { 3.3, 3.4, 3.35, 3.2 };
    for (j = 0; j < 4; j++)
    {
        glPushMatrix();
        siz -= 0.3;
        glPointSize(siz);
        glScalef(scl[j], scl[j], scl[j]);
        glBegin(GL_POINTS);
        double ang1 = 0.0 - angleAstroid, a = (2 * PI) / div;
        for (i = 0; i < div; i++)
        {
            glVertex2d(cos(ang1), sin(ang1));
            ang1 += a;
        }
        div += 10;
        glEnd();
        glPopMatrix();
    }
    glPopMatrix(); // Quỹ đạo của Mars được tạo ra

    // Hành tinh Jupiter
    glPushMatrix();
    glRotatef(angleJupiter, 0.0, 1.0, -0.5);
    glTranslatef(-4.0, 0.0, 0.0);
    glColor3f(0.4, 0.2, 0.0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(1);

    // Mặt Trăng quay quanh Sao Mộc
    glPushMatrix();
    glRotatef(angleMoon, 1.0, -0.5, 0.0);
    glTranslatef(0.0, 0, 1.1);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.1, 0.1, 0.1);
    drawSphere(0.5);
    glPopMatrix(); // Mặt Trăng được tạo ra
    glPopMatrix(); // Sao Mộc được tạo ra

    // Hành tinh Saturn
    glPushMatrix();
    glRotatef(angleSaturn, 0.0, 1.0, -1.0);
    glTranslatef(-5.0, 0.0, 0.0);
    glColor3f(0.9, 0.0, 0.0);
    glScalef(0.4, 0.4, 0.4);
    drawSphere(1);

    // Vòng đeo của Saturn
    glPushMatrix();
    glRotatef(45, 1.0, 0.0, 0.0);
    glPointSize(2);
    glColor3f(5.0, 3.0, 1.0);
    glScalef(1.2, 1.2, 1.2);
    glBegin(GL_LINE_STRIP);
    double ang1 = 0.0;
    i = 0;
    for (i = 0; i < 70; i++)
    {
        glVertex2d(cos(ang1), sin(ang1));
        ang1 += angular;
    }
    glEnd();

    glPointSize(2);
    glPopMatrix(); // Vòng đeo của Saturn được tạo ra
    glPopMatrix(); // Saturn được tạo ra

    // Hành tinh Uranus
    glPushMatrix();
    glRotatef(angleUranus, 0.0, 1.0, -0.5);
    glTranslatef(5.2, 0.0, 0.0);
    glColor3f(0.0, 0.5, 0.9);
    glScalef(0.23, 0.23, 0.23);
    drawSphere(1);
    glPopMatrix(); // Uranus được tạo ra

    // Hành tinh Neptune
    glPushMatrix();
    glRotatef(angleNeptune, 0.0, 1.0, -0.5);
    glTranslatef(-5.7, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.9);
    glScalef(0.2, 0.2, 0.2);
    drawSphere(1);
    glPopMatrix(); // Neptune được tạo ra

    glPopMatrix();
    glFlush();
}

// Hàm cập nhật trạng thái
void update(int value)
{
    // Điều chỉnh kích thước của Hệ Mặt Trời dựa trên góc quay của Mặt Trăng
    if ((angleMoon >= 0 && angleMoon < 180))
    {
        sx -= 0.0003;
        sy -= 0.0003;
        sz -= 0.0003;
    }
    else
    {
        sx += 0.0003;
        sy += 0.0003;
        sz += 0.0003;
    }

    // Cập nhật góc quay của các hành tinh và vật thể
    angleMoon += 2;
    if (angleMoon > 360)
    {
        angleMoon -= 360;
    }

    angleEarth += 0.5;
    if (angleEarth > 360)
    {
        angleEarth -= 360;
    }

    angleMercury += 2;
    if (angleMercury > 360)
    {
        angleMercury -= 360;
    }

    angleVenus += 1.2;
    if (angleVenus > 360)
    {
        angleVenus -= 360;
    }

    angleMars += 0.5;
    if (angleMars > 360)
    {
        angleMars -= 360;
    }

    angleJupiter += 0.3;
    if (angleJupiter > 360)
    {
        angleJupiter -= 360;
    }

    angleSaturn += 0.2;
    if (angleSaturn > 360)
    {
        angleSaturn -= 360;
    }

    angleUranus += 0.1;
    if (angleUranus > 360)
    {
        angleUranus -= 360;
    }

    angleNeptune += 0.05;
    if (angleNeptune > 360)
    {
        angleNeptune -= 360;
    }

    angleAstroid += 0.005;
    if (angleAstroid > 360)
    {
        angleAstroid -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Hàm xử lý sự kiện chuột
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            lastMouseX = x;
            lastMouseY = y;
            mouseDown = true;
        }
        else if (state == GLUT_UP) {
            mouseDown = false;
        }
    }
}

// Hàm xử lý sự kiện di chuyển chuột
void motion(int x, int y) {
    if (mouseDown) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;
        rotateX += deltaY;
        rotateY += deltaX;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

// Hàm xử lý sự kiện bàn phím
void keyboard(unsigned char key, int x, int y) {
    if (key == '+') {
        zoom += 0.1; // Phóng to
    }
    else if (key == '-') {
        zoom -= 0.1; // Thu nhỏ
        if (zoom < 0.1) zoom = 0.1; // Đảm bảo tỷ lệ không nhỏ hơn 0.1
    }
    glutPostRedisplay();
}

// Hàm thiết lập cửa sổ và các tham số
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(700, 700);
    glutCreateWindow("BTL");
    initLighting();
    myinit();
    glutDisplayFunc(draw);
    glutTimerFunc(16, update, 0);

    // Đăng ký các hàm xử lý sự kiện chuột và bàn phím
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}