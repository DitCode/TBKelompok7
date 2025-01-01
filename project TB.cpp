#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Variabel transformasi global
float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;

// Variabel untuk memilih objek (1 = Cheesecake, 2 = Donut, 3 = Cup Cake)
int currentObject = 1;

// Fungsi untuk mengatur pencahayaan
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPos[] = {2.0f, 2.0f, 2.0f, 1.0f};
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

// Fungsi untuk menggambar lingkaran (alas cheesecake)
void drawCircle(float radius, float height) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, height, 0.0f); // Titik tengah
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, height, z);
    }
    glEnd();
}

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
    float radius = 1.0f;
    float height = 0.5f;

    // Bagian alas cheesecake
    glColor3f(0.9f, 0.7f, 0.5f); // Warna cokelat untuk alas
    drawCircle(radius, 0.0f);

    // Bagian sisi cheesecake
    glColor3f(1.0f, 0.9f, 0.8f); // Warna krem untuk badan
    drawSide(radius, height);

    // Bagian atas cheesecake
    glColor3f(1.0f, 0.0f, 0.0f); // Warna merah muda untuk topping
    drawCircle(radius, height);
}

// Fungsi untuk menggambar meses pada torus
void drawSprinkles(float innerRadius, float outerRadius, int sprinkleCount) {
    srand(static_cast<unsigned>(time(0)));

    glPointSize(10.0f);
    glBegin(GL_POINTS);

    for (int i = 0; i < sprinkleCount; ++i) {
        // Random posisi theta dan phi
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
        float phi = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;

        // Hitung posisi di torus
        float x = cos(theta) * (outerRadius + innerRadius * cos(phi));
        float y = sin(theta) * (outerRadius + innerRadius * cos(phi));
        float z = innerRadius * sin(phi);

        // Hanya tambahkan meses pada area dengan warna ungu (gradasi t mendekati 1.0)
        float t = phi / (2.0f * M_PI);
        if (t > 0.7f) { // Area ungu
            // Tentukan warna meses (acak warna cerah)
            float r = static_cast<float>(rand()) / RAND_MAX;
            float g = static_cast<float>(rand()) / RAND_MAX;
            float b = static_cast<float>(rand()) / RAND_MAX;

            glColor3f(r, g, b);
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}

// Fungsi untuk menggambar torus (donat) dengan lapisan coklat rasa strawberry
void drawDonut() {
    float innerRadius = 0.3f; // Radius bagian dalam
    float outerRadius = 0.7f; // Radius bagian luar
    int sides = 50;          // Jumlah sisi
    int rings = 50;          // Jumlah cincin

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

                // Gradien warna coklat ke merah muda
                float t = (float)j / rings;
                glColor3f(0.8f * (1.0f - t) + 1.0f * t, 0.5f * (1.0f - t) + 0.2f * t, 0.2f * (1.0f - t) + 0.8f * t);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }

    // Tambahkan meses pada bagian ungu
    drawSprinkles(innerRadius, outerRadius, 3000);
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
    GLUquadric* quad = gluNewQuadric();

    // Gambar bagian bawah cup (coklat)
    glColor3f(0.6f, 0.3f, 0.1f); // Warna cokelat
    glPushMatrix();
    glTranslatef(0.0f, -0.6f, -0.3f); // Posisikan cup lebih rendah (turunkan cup)
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotasi agar cup menjadi horizontal, lawan arah
    gluCylinder(quad, 0.15f, 0.35f, 0.55f, 50, 50); // Membuat cup horizontal
    glPopMatrix();

    // Gambar bagian atas cup (disk)
    glPushMatrix();
    glTranslatef(0.0f, -0.06f, -0.3f); // Posisikan disk lebih rendah agar pas dengan bagian bawah cup
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotasi disk horizontal, lawan arah
    gluDisk(quad, 0.0f, 0.35f, 50, 1);
    glPopMatrix();

    // Gambar es krim vanila di dalam cup
    glColor3f(1.0f, 0.9f, 0.8f); // Warna vanila
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, -0.3f); // Geser es krim sedikit lebih bawah agar lebih pas di dalam cup
    glutSolidSphere(0.3f, 50, 50);   // Ukuran lebih kecil agar pas di dalam cup

    // Tambahkan lapisan bola es krim
    glTranslatef(0.0f, 0.25f, 0.0f);
    glutSolidSphere(0.25f, 50, 50);  // Ukuran lebih kecil agar tidak keluar dari cup

    glTranslatef(0.0f, 0.2f, 0.0f);
    glutSolidSphere(0.2f, 50, 50);  // Ukuran lebih kecil lagi agar pas di dalam cup

    // Tambahkan sprinkle di atas es krim
    drawSprinkles();

    glPopMatrix();
 
    // Tambahkan cherry di atas es krim
    glColor3f(1.0f, 0.0f, 0.0f); // Warna cherry merah
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, -0.3f); // Letakkan cherry sedikit lebih tinggi
    glutSolidSphere(0.1f, 50, 50);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

// Fungsi display untuk merender objek
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Atur posisi kamera
    gluLookAt(0.0, 0.0, 3.0,   // Posisi kamera
              0.0, 0.0, 0.0,   // Titik yang dilihat
              0.0, 1.0, 0.0);  // Vektor up

    // Terapkan transformasi
    glTranslatef(translateX, translateY, translateZ);
    glScalef(scaleX, scaleY, scaleZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);

    // Gambar objek sesuai pilihan
    if (currentObject == 1) {
        drawCheesecake();
    } else if (currentObject == 2) {
        drawDonut();
    } else if (currentObject == 3){
    	drawCupcake();
	}

    glutSwapBuffers();
}

// Fungsi untuk mengatur ulang ukuran window
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Rotasi
        case 'x': rotateX += 5.0f; break;
        case 'X': rotateX -= 5.0f; break;
        case 'y': rotateY += 5.0f; break;
        case 'Y': rotateY -= 5.0f; break;
        case 'z': rotateZ += 5.0f; break;
        case 'Z': rotateZ -= 5.0f; break;

        // Skalasi
        case 's': scaleX += 0.1f; scaleY += 0.1f; scaleZ += 0.1f; break;
        case 'S': scaleX -= 0.1f; scaleY -= 0.1f; scaleZ -= 0.1f; break;

        // Translasi
        case 't': translateX += 0.1f; break;
        case 'T': translateX -= 0.1f; break;
        case 'u': translateY += 0.1f; break;
        case 'U': translateY -= 0.1f; break;
        case 'v': translateZ += 0.1f; break;
        case 'V': translateZ -= 0.1f; break;

        // Keluar
        case 27: exit(0); break; // Escape key
    }
    glutPostRedisplay();
}

// Fungsi untuk memilih objek dari menu
void menu(int value) {
    switch (value) {
        case 1: // Cheesecake
            currentObject = 1;
            break;
        case 2: // Donut
            currentObject = 2;
            break;
        case 3: // Cupcake
        	currentObject = 3;
        	break;
        case 4: // Exit
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// Fungsi untuk menampilkan menu konteks
void createMenu() {
    int menuId = glutCreateMenu(menu);
    glutAddMenuEntry("Cheesecake", 1);
    glutAddMenuEntry("Donut", 2);
    glutAddMenuEntry("Cupcake", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Menambahkan menu pada klik kanan
}

// Fungsi utama
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
