#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Variabel transformasi global
float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;

void setupLighting();
void drawCircle(float radius, float height, float r, float g, float b);
void drawPlate();
void drawSide(float radius, float height);
void drawCheesecake();
void drawDonut();
void drawSprinkles();
void drawCupcake();
void display();
void reshape(int w, int h);
void drawLight();
void keyboard(unsigned char key, int x, int y);
void menu(int value);
void createMenu();
int main(int argc, char** argv);

// Variabel untuk kontrol pencahayaan
float lightPosX = 100.0f;
float lightPosY = -100.0f;
float lightPosZ = 100.0f;
	
// Variabel untuk memilih objek (1 = Cheesecake, 2 = Donut, 3 = Cup Cake)
int currentObject = 1;

// Muhammad Aditya Heryadi
// Fungsi untuk mengatur pencahayaan
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPos[] = {0.0f, 5.0f, 5.0f, 0.0f};
    float lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// Fungsi untuk menggambar torus (donat)
void drawDonut() {
    drawPlate(); // Tambahkan piring

    float innerRadius = 0.3f;
    float outerRadius = 0.4f;
    int sides = 50;
    int rings = 50;
	
	glPushMatrix(); // Simpan transformasi sebelumnya
    glTranslatef(0.0f, 0.25f, 0.0f); // Geser donut ke atas (sumbu Y)
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotasi donut agar horizontal
    glScalef(1.0f, 1.0f, 1.0f); // Skala donut di semua sumbu (X, Y, Z)

    for (int i = 0; i < sides; ++i) {
        float theta1 = (float)i / sides * 2.0f * M_PI;
        float theta2 = (float)(i + 1) / sides * 2.0f * M_PI;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= rings; ++j) {
            float phi = (float)j / rings * 2.0f * M_PI;

            for (int k = 0; k < 2; ++k) {
                float theta = (k == 0) ? theta1 : theta2;
                float x = cos(theta) * (outerRadius + innerRadius * cos(phi));
                float y = sin(theta) * (outerRadius + innerRadius * cos(phi));
                float z = innerRadius * sin(phi);

                float t = (float)j / rings;
                glColor3f(0.8f * (2.0f - t) + 1.0f * t, 0.5f * (1.0f - t) + 0.2f * t, 0.2f * (1.0f - t) + 0.8f * t);
                glVertex3f(x, y, z);
         
		    }
        }
        glEnd();
    }

    
    glPopMatrix(); // Kembalikan transformasi sebelumnya
}

void display() {
	glClearColor(0.0f, 0.0f, 9.5f, 1.0f); // Ubah latar belakang
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
     // Gambar sumbu X (merah)
    glColor3f(1.0, 0.0, 0.0); // Warna merah
    glBegin(GL_LINES);
        glVertex3f(-10.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);
    glEnd();

    // Gambar sumbu Y (hijau)
    glColor3f(0.0, 1.0, 0.0); // Warna hijau
    glBegin(GL_LINES);
        glVertex3f(0.0, -10.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
    glEnd();


    
     // Atur posisi pencahayaan
    GLfloat lightPosition[] = {lightPosX, lightPosY, lightPosZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glTranslatef(translateX, translateY, translateZ);
    glScalef(scaleX, scaleY, scaleZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    if (currentObject == 1) {
        drawCheesecake();
    } else if (currentObject == 2) {
        drawDonut();
    } else if (currentObject == 3) {
        drawCupcake();
    }

    glutSwapBuffers();
}

void drawLight()
{
    glDisable(GL_LIGHTING); // Matikan pencahayaan 
    glPushMatrix();

    // Posisikan bola matahari di lokasi cahaya
    glTranslatef(lightPosX, lightPosY, lightPosZ);

    // Atur warna tetap untuk cahaya (oranye)
    glColor3f(1.0f, 0.7f, 0.0f); // Warna oranye kekuningan untuk matahari
    glutSolidSphere(5.0, 20, 20); // Menggambar bola sebagai representasi cahaya

    glPopMatrix();
    glEnable(GL_LIGHTING); // Aktifkan kembali pencahayaan
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// Bambang Surya Prana 
// Fungsi untuk menggambar lingkaran (piring atau alas lainnya)
void drawCircle(float radius, float height, float r, float g, float b) {
    glColor3f(r, g, b); // Warna lingkaran
    glBegin(GL_TRIANGLE_FAN); // Memulai menggambar dengan mode TRIANGLE_FAN
    glVertex3f(0.0f, height, 0.0f); // Titik tengah lingkaran
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f; // Menghitung sudut dalam radian
        float x = radius * cos(angle); // Menghitung posisi x berdasarkan radius dan sudut
        float z = radius * sin(angle); // Menghitung posisi z berdasarkan radius dan sudut
        glVertex3f(x, height, z); // Menambahkan titik pada lingkaran
    }
    glEnd(); // Mengakhiri proses menggambar
}

// Fungsi untuk menggambar piring
void drawPlate() {
	// Geser piring ke posisi yang diinginkan
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);  // Geser piring ke posisi yang diinginkan
    glScalef(1.0f, 1.0f, 1.0f); // Skala piring di semua sumbu (X, Y, Z)
    
    GLUquadric* quad = gluNewQuadric();

    // Rotasi piring agar horizontal
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotasi 90 derajat di sumbu X

    // Bagian luar piring (silinder untuk ketebalan)
    glColor3f(0.9f, 0.9f, 0.9f); // Warna abu-abu terang
    glPushMatrix();
    gluCylinder(quad, 1.0f, 1.0f, 0.05f, 50, 50); // Silinder dengan tinggi 0.05
    glPopMatrix();

    // Bagian atas piring (lingkaran atas)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.05f); // Geser ke atas untuk tutup silinder
    gluDisk(quad, 0.0f, 1.0f, 50, 1);
    glPopMatrix();

    // Bagian bawah piring (lingkaran bawah)
    glPushMatrix();
    gluDisk(quad, 0.0f, 1.0f, 50, 1); // Lingkaran bawah tidak perlu translasi
    glPopMatrix();

    glPopMatrix(); // Selesai rotasi piring

    gluDeleteQuadric(quad);
    
     glPopMatrix();  // Kembali ke transformasi sebelumnya
}

// Fungsi untuk menggambar sprinkle di atas es krim
void drawSprinkles() {
    glColor3f(1.0f, 0.0f, 0.0f); // Warna sprinkle merah
    for (int i = 0; i < 20; i++) {
        glPushMatrix();
        float x = (rand() % 100 - 50) / 100.0f * 0.5f;
        float y = (rand() % 100 - 50) / 100.0f * 0.5f;
        float z = (rand() % 100 - 50) / 100.0f * 0.5f;
        glTranslatef(x, y + -0.1f, z); // Posisi sprinkle di atas es krim
        glutSolidSphere(0.02f, 10, 10);
        glPopMatrix();
    }
}

void drawCupcake() {  
	drawPlate(); // Tambahkan piring
	
    GLUquadric* quad = gluNewQuadric();
    
    // Geser cupcake ke posisi yang diinginkan
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);  // Geser cupcake ke posisi yang diinginkan 
    glScalef(1.0f, 1.0f, 1.0f); // Skala cupcake di semua sumbu (X, Y, Z)

    // Gambar bagian bawah cup (coklat)
    glColor3f(0.6f, 0.3f, 0.1f); // Warna cokelat
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, -0.3f); // Posisikan cup lebih rendah (turunkan cup)
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotasi agar cup menjadi horizontal, lawan arah
    gluCylinder(quad, 0.15f, 0.35f, 0.55f, 50, 50); // Membuat cup horizontal
    glPopMatrix();

    // Gambar bagian atas cup (disk)
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -0.3f); // Posisikan disk lebih rendah agar pas dengan bagian bawah cup
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotasi disk horizontal, lawan arah
    gluDisk(quad, 0.0f, 0.35f, 50, 1);
    glPopMatrix();

    // Gambar es krim vanila di dalam cup
    glColor3f(1.0f, 0.9f, 0.8f); // Warna vanila
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -0.3f); // Geser es krim sedikit lebih bawah agar lebih pas di dalam cup
    glutSolidSphere(0.3f, 50, 50);   // Ukuran lebih kecil agar pas di dalam cup

    // Tambahkan lapisan bola es krim
    glTranslatef(0.0f, 0.25f, 0.0f);
    glutSolidSphere(0.20f, 50, 50);  // Ukuran lebih kecil agar tidak keluar dari cup

    glTranslatef(0.0f, 0.2f, 0.0f);
    glutSolidSphere(0.15f, 50, 50);  // Ukuran lebih kecil lagi agar pas di dalam cup

    // Tambahkan sprinkle di atas es krim
    drawSprinkles();

    glPopMatrix();
 
    // Tambahkan cherry di atas es krim
    glColor3f(1.0f, 0.0f, 0.0f); // Warna cherry merah
    glPushMatrix();
    glTranslatef(0.0f, 1.1f, -0.3f); // Letakkan cherry sedikit lebih tinggi
    glutSolidSphere(0.05f, 50, 50);
    glPopMatrix();

    gluDeleteQuadric(quad);
    
    glPopMatrix();  // Kembali ke transformasi sebelumnya
}

void menu(int value) {
    switch (value) {
        case 1: currentObject = 1; break;
        case 2: currentObject = 2; break;
        case 3: currentObject = 3; break;
        case 4: exit(0); break;
    }
    glutPostRedisplay();
}

void createMenu() {
    int menuId = glutCreateMenu(menu);
    glutAddMenuEntry("Cheesecake", 1);
    glutAddMenuEntry("Donut", 2);
    glutAddMenuEntry("Cupcake", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Vidya Tiara Eka Putri
// Fungsi untuk menggambar sisi cheesecake
void drawSide(float radius, float height) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();
}

// Fungsi untuk menggambar cheesecake
void drawCheesecake() {
    drawPlate(); // Tambahkan piring
    
    glPushMatrix(); // Simpan transformasi sebelumnya
    glTranslatef(0.0f, -0.04f, 0.0f); // Geser donut ke atas (sumbu Y)
    glScalef(1.0f, 1.0f, 1.0f); // Skala cheesecake di semua sumbu (X, Y, Z)

    float radius = 0.7f;
    float height = 0.5f;

    // Bagian alas cheesecake
    glColor3f(0.9f, 0.7f, 0.5f); // Warna cokelat untuk alas
    drawCircle(radius, 0.0f, 0.9f, 0.7f, 0.5f);

    // Bagian sisi cheesecake
    glColor3f(1.0f, 0.9f, 0.8f); // Warna krem untuk badan
    drawSide(radius, height);

    // Bagian atas cheesecake
    glColor3f(1.0f, 0.0f, 0.0f); // Warna merah muda untuk topping
    drawCircle(radius, height, 1.0f, 0.0f, 0.0f);
    
    glPopMatrix(); // Kembalikan transformasi sebelumnya
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x': rotateX += 5.0f; break;
        case 'X': rotateX -= 5.0f; break;
        case 'y': rotateY += 5.0f; break;
        case 'Y': rotateY -= 5.0f; break;
        case 'z': rotateZ += 5.0f; break;
        case 'Z': rotateZ -= 5.0f; break;
        case 's': scaleX += 0.1f; scaleY += 0.1f; scaleZ += 0.1f; break;
        case 'S': scaleX -= 0.1f; scaleY -= 0.1f; scaleZ -= 0.1f; break;
        case 't': translateX += 0.1f; break;
        case 'T': translateX -= 0.1f; break;
        case 'u': translateY += 0.1f; break;
        case 'U': translateY -= 0.1f; break;
        case 'v': translateZ += 0.1f; break;
        case 'V': translateZ -= 0.1f; break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

//Bebas
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cheesecake or Donut or Cupcake");

    glEnable(GL_DEPTH_TEST);
    setupLighting();

    createMenu();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
