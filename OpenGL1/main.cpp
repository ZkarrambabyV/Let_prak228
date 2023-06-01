#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <list>
#include <vector>

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

void showList(list<double> list)
{
	cout << endl;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		cout << *it << " ";
	}
}

void drawExperiment(GLFWwindow* window, double a, double b, list<double> x, list<double> y) 
{
	double up_border = 2;
	double down_border = -2;
	double left_border = -2;
	double right_border = 5;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(left_border, right_border, down_border, up_border, -1.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(left_border, 0);
	glVertex2f(right_border, 0);
	glVertex2f(0, down_border);
	glVertex2f(0, up_border);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	list<double>::const_iterator it1 = x.begin();
	list<double>::const_iterator it2 = y.begin();
	glBegin(GL_LINE_STRIP);
	for (; it1 != x.end() && it2 != y.end(); it1++, it2++)
	{
		glVertex2f(*it1, *it2);
	}
	glEnd();
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void drawRegress(GLFWwindow* window, double a, double b)
{
	double up_border = 2;
	double down_border = -2;
	double left_border = -2;
	double right_border = 2;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(left_border, right_border, down_border, up_border, -1.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(left_border, 0);
	glVertex2f(right_border, 0);
	glVertex2f(0, down_border);
	glVertex2f(0, up_border);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (double x = left_border; x <= right_border; x += 0.01)
	{
		double y = (x) / (a * x + b);
		glVertex2f(x, y);
	}
	glEnd();
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void drawPredY(GLFWwindow* window, list<double> x, list<double> predY) 
{
	double up_border = 2;
	double down_border = -2;
	double left_border = -2;
	double right_border = 5;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(left_border, right_border, down_border, up_border, -1.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(left_border, 0);
	glVertex2f(right_border, 0);
	glVertex2f(0, down_border);
	glVertex2f(0, up_border);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	list<double>::const_iterator it1 = x.begin();
	list<double>::const_iterator it2 = predY.begin();
	glBegin(GL_LINE_STRIP);
	for (; it1 != x.end() && it2 != predY.end(); it1++, it2++)
	{
		glVertex2f(*it1, *it2);
	}
	glEnd();
	glfwPollEvents();
	glfwSwapBuffers(window);
}

double korKoef(list<double> x, list<double> y)
{
	double sumX = 0;
	double sumY = 0;
	double sumXy = 0;
	double sumX2 = 0;
	double sumY2 = 0;
	int n = 9;
	list<double>::const_iterator it3 = x.begin();
	list<double>::const_iterator it4 = y.begin();
	for (; it3 != x.end() && it4 != y.end(); it3++, it4++)
	{
		sumXy += ((*it3) * (*it4));
	}
	for (auto it = x.begin(); it != x.end(); it++)
	{
		sumX += *it;
		sumX2 += (*it) * (*it);
	}
	for (auto it = y.begin(); it != y.end(); it++)
	{
		sumY += *it;
		sumY2 += (*it) * (*it);
	}
	double numer = (n * sumXy) - (sumX * sumY);
	double denomin = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

	if (denomin == 0) {
		return 0;
	}

	return numer / denomin;
}

double regKoef(double xAvg, double yAvg, list<double> x, list<double> y)
{
	int n = 9;
	double dx = 0;
	double covar = 0;
	list<double>::const_iterator it1 = x.begin();
	list<double>::const_iterator it2 = y.begin();
	for (; it1 != x.end() && it2 != y.end(); it1++, it2++)
	{
		dx += pow(*it1 - xAvg, 2);
		covar += (*it1 - xAvg) * (*it2 - yAvg);
	}
	dx /= n;
	covar /= n;

	return covar / dx;
}

double SumErr2Calc(list<double> y, list<double> predY)
{
	double sum = 0;
	list<double>::const_iterator it1 = y.begin();
	list<double>::const_iterator it2 = predY.begin();
	for (; it1 != y.end() && it2 != predY.end(); it1++, it2++)
	{
		sum += pow(*it1 - *it2, 2);
	}
	return sum;
}

void main()
{
	if (!glfwInit())
	{
		glfwTerminate();
	}
	GLFWwindow *window = glfwCreateWindow(640, 480, "Graphik", NULL, NULL);
	if (!window) {
		glfwTerminate();
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	list<double> x = { 1, 1.5 , 2, 2.5, 3, 3.5, 4, 4.5, 5 };
	list<double> y = { 0.2140, 0.2210, 0.2237, 0.2258, 0.2262, 0.2262, 0.2275, 0.2283, 0.2288 };
	list<double> dx;
	list<double> dy;
	list<double> predY;
	double sumDxy2 = 0;
	double xAvg = 0;
	double yAvg = 0;
	int n = 0;
	double sumDx2 = 0;
	double sumDy2 = 0;
	double sumX = 0;
	double sumY = 0;
	double sumXy = 0;
	double inter = 0;
	cout << "x : ";
	for (auto it = x.begin(); it != x.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl << "y : ";
	for (auto it = y.begin(); it != y.end(); it++)
	{
		cout << *it << " ";
	}
	for (auto it = x.begin(); it != x.end(); it++)
	{
		xAvg += *it;
		n++;
	}
	for (auto it = y.begin(); it != y.end(); it++)
	{
		yAvg += *it;
	}
	xAvg /= n;
	yAvg /= n;
	cout << endl << "Srednee Znachenie x = " << xAvg << endl;
	cout << "Srednee Znachenie y = " << yAvg << endl;
	for (auto it = x.begin(); it != x.end(); it++)
	{
		double temp = *it - xAvg;
		dx.push_back(temp);
	}
	for (auto it = y.begin(); it != y.end(); it++)
	{
		double temp = *it - yAvg;
		dy.push_back(temp);
	}
	cout << "Spisok otkloneniy x: ";
	showList(dx);
	cout << endl << "Spisok otkloneniy y: ";
	showList(dy);
	for (auto it = dx.begin(); it != dx.end(); it++)
	{
		sumDx2 += pow(*it, 2);
	}
	for (auto it = dy.begin(); it != dy.end(); it++)
	{
		sumDy2 += pow(*it, 2);
	}
	cout << endl << "Summa kVadratoV otkloneniy x: " << sumDx2 << endl;
	cout << "Summa kVadratoV otkloneniy y: " << sumDy2 << endl;
	list<double>::const_iterator it1 = dx.begin();
	list<double>::const_iterator it2 = dy.begin();
	for (;it1 != dx.end() && it2 != dy.end(); it1++, it2++)
	{
		sumDxy2 += pow(((*it1) * (*it2)), 2);
	}
	cout << "Summa kVadratoV otkloneniy proiZVedeniy x y = " << sumDxy2 << endl;
	double a = sumDxy2 / sumDx2;
	double b = yAvg - a * xAvg;
	cout << "Koeffitsient a = " << a << endl;
	cout << "Koeffitsient b = " << b << endl;

	double koefKor = korKoef(x, y);

	cout << "Koeffitsient korrilyatsii = " << koefKor << endl;

	double koefReg = regKoef(xAvg, yAvg, x, y);

	cout << "Koeffitsient regressii = " << koefReg << endl;

	inter = yAvg - koefReg * xAvg;

	for (auto it = x.begin(); it != x.end(); it++)
	{
		predY.push_back(koefReg * (*it) + inter);
	}
	cout << "PredskaZannie Znacheniya y : ";
	showList(predY);
	double sumErr2 = SumErr2Calc(y, predY);
	cout << endl << "Summarnaya kVadratichnaya oshibka = " << sumErr2 << endl;
	double midErr = sqrt(sumErr2 / 9);
	cout << "Srednyaya oshibka = " << midErr << endl;
	int press = 0;

	cout << "Dlya ViVoda grafika eksperementalnoy Zavisimosti, naZmite 1 " << endl;
	cout << "Dlya ViVoda grafika podobrannoy ZaVisimosti, naZmite 2 " << endl;
	cout << "Dlya ViVoda linii regressii, naZmite 3 " << endl;
	cout << "Dlya Vihoda iZ programmi, naZmite Ecs";
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_1)) {
			press = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_2)) {
			press = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_3)) {
			press = 3;
		}
		if (press == 1) {
			glClear(GL_COLOR_BUFFER_BIT);
			drawExperiment(window, a, b, x, y);
		}
		if (press == 2) {
			glClear(GL_COLOR_BUFFER_BIT);
			drawPredY(window, x, predY);
		}
		if (press == 3) {
			glClear(GL_COLOR_BUFFER_BIT);
			drawRegress(window, a, b);
		}
	}
	glfwTerminate();
}