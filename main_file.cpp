/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.
Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.
Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "allmodels.h"
#include "alltextures.h"
#include "camera.h"
#include <iostream>

Camera* kamera;
Kadlub* kadlub;
Wieza* wieza;
Lufa* lufa;
Ziemia* ziemia;
Niebo* niebo;
Gasiennica* gasiennicaL;
Gasiennica* gasiennicaP;

KoloS* koloSLp;
KoloS* koloSLt;
KoloS* koloSPp;
KoloS* koloSPt;

KoloL* koloLL1;
KoloL* koloLL2;
KoloL* koloLL3;
KoloL* koloLL4;
KoloL* koloLL5;

KoloL* koloP1;
KoloL* koloP2;
KoloL* koloP3;
KoloL* koloP4;
KoloL* koloP5;

Pocisk* pocisk;

std::vector<Model*> modele;
std::vector<Pocisk*> aktywnePociski;
std::vector<KoloL*> kolaLewe;
std::vector<KoloL*> kolaPrawe;
std::vector<KoloS*> kolaMaleL;
std::vector<KoloS*> kolaMaleP;

float aspectRatio = 1;
float speed_x = 0,speed_y = 0;
float predkoscJazdy = 0;
float obrotWiezy = 0, podniesienie = 0;
float skret = 0;
float fov = 45.0f, theta = 0, phi = 0, radius = 0;

ShaderProgram* sp;
ShaderProgram* sp_niebo;

//Uchwyty na tekstury
GLuint texZiemia;
GLuint texKadlub;
GLuint texWieza;
GLuint texBloczek;
GLuint texGasiennica;
GLuint texKolo;
GLuint texNiebo;
GLuint texPocisk;

// Ustawienie klawiszy kamery
bool up_pressed = false;
bool down_pressed = false;
bool left_pressed = false;
bool right_pressed = false;
// Ustawienie klawiszy czołgu
bool w_pressed = false;
bool tank_left = false;
bool s_pressed = false;
bool tank_right = false;
bool turret_left = false;
bool turret_right = false;
bool r_pressed = false;
bool f_pressed = false;
// Strzał
bool space_pressed = false;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_UP) up_pressed = true;
		if (key == GLFW_KEY_DOWN) down_pressed = true;
		if (key == GLFW_KEY_LEFT) left_pressed = true;
		if (key == GLFW_KEY_RIGHT) right_pressed = true;
		if (key == GLFW_KEY_W) w_pressed = true;
		if (key == GLFW_KEY_A) tank_left = true;
		if (key == GLFW_KEY_S) s_pressed = true;
		if (key == GLFW_KEY_D) tank_right = true;
		if (key == GLFW_KEY_Q) turret_left = true;
		if (key == GLFW_KEY_E) turret_right = true;
		if (key == GLFW_KEY_R) r_pressed = true;
		if (key == GLFW_KEY_F) f_pressed = true;
		if (key == GLFW_KEY_SPACE) space_pressed = true;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_UP) up_pressed = false;
		if (key == GLFW_KEY_DOWN) down_pressed = false;
		if (key == GLFW_KEY_LEFT) left_pressed = false;
		if (key == GLFW_KEY_RIGHT) right_pressed = false;
		if (key == GLFW_KEY_W) w_pressed = false;
		if (key == GLFW_KEY_A) tank_left = false;
		if (key == GLFW_KEY_S) s_pressed = false;
		if (key == GLFW_KEY_D) tank_right = false;
		if (key == GLFW_KEY_Q) turret_left = false;
		if (key == GLFW_KEY_E) turret_right = false;
		if (key == GLFW_KEY_R) r_pressed = false;
		if (key == GLFW_KEY_F) f_pressed = false;
		if (key == GLFW_KEY_SPACE) space_pressed = false;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 90.0f)
		fov = 90.0f;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void wczytajModele() {
	kamera = new Camera;
	kadlub = new Kadlub();
	wieza = new Wieza();
	lufa = new Lufa();
	ziemia = new Ziemia();
	niebo = new Niebo();
	gasiennicaL = new Gasiennica(vec3(1.45f, 0.1f, 0.0f));// Lewa 
	gasiennicaP = new Gasiennica(vec3(-1.35f, 0.1f, 0.0f)); //Prawa

	//Koła skrajne małe
	koloSLp = new KoloS(vec3(1.45f, 0.1f, 0.0f));
	koloSLt = new KoloS(vec3(1.45f, 0.1 + 6.201, 0.0f));
	koloSPp = new KoloS(vec3(-1.35f, 0.1, 0.0f));
	koloSPt = new KoloS(vec3(-1.35f, 0.1 + 6.201, 0.0f));

	//Koła środek duże
	koloLL1 = new KoloL(vec3(1.45f, -2.1f - 0.15f, 0.0f));
	koloLL2 = new KoloL(vec3(1.45f, -0.98f - 0.15f, 0.0f));
	koloLL3 = new KoloL(vec3(1.45f, 0.21f - 0.15f, 0.0f));
	koloLL4 = new KoloL(vec3(1.45f, 1.21f - 0.15f, 0.0f));
	koloLL5 = new KoloL(vec3(1.45f, 2.29f - 0.15f, 0.0f));

	koloP1 = new KoloL(vec3(-1.35f, -2.1f - 0.15f, 0.0f));
	koloP2 = new KoloL(vec3(-1.35f, -0.98f - 0.15f, 0.0f));
	koloP3 = new KoloL(vec3(-1.35f, 0.21f - 0.15f, 0.0f));
	koloP4 = new KoloL(vec3(-1.35f, 1.21f - 0.15f, 0.0f));
	koloP5 = new KoloL(vec3(-1.35f, 2.29f - 0.15f, 0.0f));

	modele.push_back(niebo);
	modele.push_back(kadlub);
	modele.push_back(wieza);
	modele.push_back(lufa);
	modele.push_back(gasiennicaL);
	modele.push_back(gasiennicaP);

	modele.push_back(koloSLp);
	modele.push_back(koloSLt);
	modele.push_back(koloSPp);
	modele.push_back(koloSPt);

	modele.push_back(koloLL1);
	modele.push_back(koloLL2);
	modele.push_back(koloLL3);
	modele.push_back(koloLL4);
	modele.push_back(koloLL5);

	modele.push_back(koloP1);
	modele.push_back(koloP2);
	modele.push_back(koloP3);
	modele.push_back(koloP4);
	modele.push_back(koloP5);

	kolaMaleL.push_back(koloSLp);
	kolaMaleL.push_back(koloSLt);
	kolaMaleP.push_back(koloSPp);
	kolaMaleP.push_back(koloSPt);

	kolaLewe.push_back(koloLL1);
	kolaLewe.push_back(koloLL2);
	kolaLewe.push_back(koloLL3);
	kolaLewe.push_back(koloLL4);
	kolaLewe.push_back(koloLL5);

	kolaPrawe.push_back(koloP1);
	kolaPrawe.push_back(koloP2);
	kolaPrawe.push_back(koloP3);
	kolaPrawe.push_back(koloP4);
	kolaPrawe.push_back(koloP5);
}

//Funkcja wczytująca teksturę
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_NORMAL);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);	
	glfwSetScrollCallback(window, scrollCallback);
	
	wczytajModele();

	sp = new ShaderProgram("vertex.glsl", NULL, "fragment.glsl");
	sp_niebo = new ShaderProgram("vertex_niebo.glsl", NULL, "fragment_niebo.glsl");

	texZiemia = readTexture(tekstura_ziemia);
	texKadlub = readTexture(tekstura_kadlub);
	texWieza = readTexture(tekstura_wieza);
	texBloczek = readTexture(tekstura_bloczek);
	texGasiennica = readTexture(tekstura_gasiennica);
	texKolo = readTexture(tekstura_kolo);
	texNiebo = readTexture(tekstura_niebo);
	texPocisk = readTexture(tekstura_pocisk);
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &texZiemia);
	glDeleteTextures(1, &texKadlub);
	glDeleteTextures(1, &texWieza);
	glDeleteTextures(1, &texBloczek);
	glDeleteTextures(1, &texGasiennica);
	glDeleteTextures(1, &texKolo);
	glDeleteTextures(1, &texNiebo);
	glDeleteTextures(1, &texPocisk);
	delete sp;
	delete sp_niebo;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle, float wheelL, float wheelP, float obrotWieza, float depression, float ruchNieba, float s, float t, float r) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	kamera->findPointOnSphere(s, t, r);
	kamera->przesunDoModelu(kadlub->position[0], kadlub->position[2], kadlub->position[1]);

	glm::mat4 V = glm::lookAt(
		glm::vec3(kamera->x, kamera->y, kamera->z),
		glm::vec3(kadlub->position[0], -kadlub->position[2], -kadlub->position[1]),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania 

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniform4f(sp->u("lp[0]"), 8000, 10000, 0, 1); //Współrzędne źródła światła
	glUniform4f(sp->u("lp[1]"), -800, 5, 0, 1);
	glUniform4f(sp->u("lp[2]"), 0, 5, 800, 1);
	glUniform4f(sp->u("lp[3]"), 0, 5, -800, 1);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);

	kadlub->drawKadlub(texKadlub, sp);

	wieza->obrot = obrotWieza;
	lufa->obrot = obrotWieza;
	lufa->podniesienie = depression;
	wieza->drawWieza(texWieza, sp);
	lufa->drawLufa(texWieza, sp);
	ziemia->drawZiemia(texZiemia, sp);

	gasiennicaL->drawGasienica(texGasiennica, sp);
	gasiennicaP->drawGasienica(texGasiennica, sp);

	for (int i = 0; i < kolaLewe.size(); i++) {
		kolaLewe[i]->obrot = wheelL;
		kolaPrawe[i]->obrot = wheelP;
		kolaLewe[i]->drawKoloL(texKolo, sp);
		kolaPrawe[i]->drawKoloL(texKolo, sp);
	}

	for (int i = 0; i < 2; i++) {
		kolaMaleL[i]->obrot = wheelL;
		kolaMaleP[i]->obrot = wheelP;
		kolaMaleL[i]->drawKoloS(texKolo, sp);
		kolaMaleP[i]->drawKoloS(texKolo, sp);
	}


	sp_niebo->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp_niebo->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp_niebo->u("V"), 1, false, glm::value_ptr(V));
	glUniform4f(sp_niebo->u("lp[0]"), 0, 0, 1000, 1);
	glUniform4f(sp_niebo->u("lp[1]"), 0, 0, -1000, 1);
	glUniform4f(sp_niebo->u("lp[2]"), 1000, 0, 0, 1);
	glUniform4f(sp_niebo->u("lp[3]"), -1000, 0, 0, 1);
	glUniform4f(sp_niebo->u("lp[4]"), 1000, 0, 1000, 1);
	glUniform4f(sp_niebo->u("lp[5]"), -1000, 0, 1000, 1);
	glUniform4f(sp_niebo->u("lp[6]"), 1000, 0, -1000, 1);
	glUniform4f(sp_niebo->u("lp[7]"), -1000, 0, -1000, 1);

	glUniform1i(sp_niebo->u("textureMap0"), 0);
	glUniform1i(sp_niebo->u("textureMap1"), 1);

	niebo->angleX = ruchNieba;
	niebo->drawNiebo(texNiebo, sp_niebo);

	// Renderuj istniejące pociski
	if (aktywnePociski.size() > 0)
	{
		for (auto& pocisk : aktywnePociski)
		{
			pocisk->Update(depression);
			if (pocisk->life < 0) aktywnePociski.erase(aktywnePociski.begin());
			else pocisk->drawPocisk(texPocisk, sp);
		}
	}
	
	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

void obsluzKlikniecie(float angle) {
	// strzelanie
	if (space_pressed) {
		pocisk = new Pocisk(lufa->position, (kadlub->angleZ * PI / 180) + 2 * PI + lufa->obrot);
		aktywnePociski.push_back(pocisk);
		space_pressed = false;
	}

	//oblicz predkosc jazdy
	if (w_pressed) {
		if (predkoscJazdy < 2 * PI) predkoscJazdy += (2 * PI * 1 / 120); // Ograniczenie predkosci
	}
	else if (s_pressed) {
		if (predkoscJazdy > 2 * PI * 1 / 70) predkoscJazdy -= (2 * PI * 1 / 70);
		else if (predkoscJazdy > 1.5 * -PI) predkoscJazdy -= (2 * PI * 1 / 120);
	}
	else {
		if (predkoscJazdy > 2 * PI * 1 / 200) predkoscJazdy -= (2 * PI * 1 / 200); // Powolne zmniejszanie prędkości
		else if (predkoscJazdy < -2 * PI * 1 / 70) predkoscJazdy += (2 * PI * 1 / 70);
		else predkoscJazdy = 0;
	}

	//obroc wieze
	if (turret_left) obrotWiezy = PI / 4;
	else if (turret_right) obrotWiezy = -PI / 4;
	else obrotWiezy = 0;

	//podnies/obniz lufe
	if (f_pressed) {
		if (lufa->podniesienie < 2 * PI / 60) podniesienie = 2 * PI * 1 / 80;
		else podniesienie = 0;
	}
	else if (r_pressed) {
		if (lufa->podniesienie > -2 * PI / 60) podniesienie = -2 * PI * 1 / 80;
		else podniesienie = 0;
	}
	else podniesienie = 0;
	

	//obrot czolgu
	if (tank_left) {
		skret = -15;
	}
	else if (tank_right) {
		skret = 15;
	}
	else skret = 0;

	//ustal przesuniecie czolgu na osiach x i y
	float ruchx;
	float ruchy;

	if (angle >= 0 && angle < 90) {
		ruchx = (1 - (angle / 90)) * predkoscJazdy / 100;
		ruchy = (angle / 90) * predkoscJazdy / 100;
	}
	else if (angle >= 90 && angle < 180) {
		ruchx = -1 * ((angle - 90) / 90) * predkoscJazdy / 100;
		ruchy = (1 - ((angle - 90) / 90)) * predkoscJazdy / 100;
	}
	else if (angle >= 180 && angle < 270) {
		ruchx = -1 * (1 - ((angle - 180) / 90)) * predkoscJazdy / 100;
		ruchy = -1 * ((angle - 180) / 90) * predkoscJazdy / 100;
	}
	else {
		ruchx = (angle - 270) / 90 * predkoscJazdy / 100;
		ruchy = -1 * (1 - ((angle - 270) / 90)) * predkoscJazdy / 100;
	}

	for (int i = 0; i < modele.size(); i++) {
		modele[i]->position[1] -= ruchx;
		modele[i]->position[0] -= ruchy;
		modele[i]->angleZ = -angle;
	}

	//oblicz przesuniecie kamery
	if (up_pressed) {
		if (kamera->t < -2 * PI / 16)
			phi = +Vcamera;
		else phi = 0;
	}
	else if (down_pressed) {
		if (kamera->t > -2 * PI / 4)
			phi = -Vcamera;
		else phi = 0;
	}
	else phi = 0;

	if (right_pressed) theta = -Vcamera;
	else if (left_pressed) theta = +Vcamera;
	else theta = 0;

}


int main(void)
{
	
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Tank Simulator", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float wheelP = 0, wieza = 0, wheelL = 0, angle = 0;
	float depression = 0, ruchNieba = 0;
	float s = 2 * PI / 4, t = -2 * PI / 5, r = 10.0f;
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		obsluzKlikniecie(angle);
		angle += skret * glfwGetTime();
		if (angle < 0) angle += 360;
		if (angle > 360) angle -= 360;

		if (predkoscJazdy != 0) {
			wheelL += predkoscJazdy * glfwGetTime();
			wheelP += predkoscJazdy * glfwGetTime();
		}
		else if (tank_left) {
			wheelL += -2 * PI * 1 / 10 * glfwGetTime();
			wheelP += 2 * PI * 1 / 10 * glfwGetTime();
		}
		else if (tank_right) {
			wheelL += 2 * PI * 1 / 10 * glfwGetTime();
			wheelP += -2 * PI * 1 / 10 * glfwGetTime();
		}

		wieza += obrotWiezy * glfwGetTime();
		depression += podniesienie * glfwGetTime();

		//kat obrotu sfery nieba
		ruchNieba += 2 * PI * 1 / 500 * glfwGetTime();
		if (ruchNieba > 2 * PI) ruchNieba -= 2 * PI;

		//kat obrotu kamery
		s += theta * glfwGetTime();
		t += phi * glfwGetTime();
		r += radius * glfwGetTime();

		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle, wheelL, wheelP, wieza, depression, ruchNieba, s, t, r); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}