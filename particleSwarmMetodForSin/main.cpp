#include <iostream>
#include <vector>
#include <cmath>
#include <GLUT/GLUT.h>
#include <time.h>
using namespace std;

class Particle{
private:
    vector<double> alpha;
public:
    Particle(vector<double>);
    Particle(int);
    Particle();
    vector<double> getCoordinates();
    double getCoordinate(int);
    void putCoordinate (int, double);
    Particle operator+(Particle P);
    Particle operator*(Particle P);
    Particle operator*(int p);
    Particle operator-(Particle P);
    void Show();
    void draw();
};
const bool operator== (Particle P, double p);
const bool operator== (double p, Particle P);
const bool operator== (Particle P1, Particle P2);
const bool operator!= (Particle P1, Particle P2);
const bool operator< (Particle P1, double p);
const bool operator> (Particle P1, double p);
double d(Particle P);
class Swarm{
private:
    vector<Particle> Part;
    vector<Particle> best,deltaXPrevious;
    Particle greatBestParticle;
    int nIteration;
public:
    Particle getGreatBest(){return greatBestParticle;};
    Swarm(vector<Particle>);
    Swarm(int,int);
    Particle greatBest();
    void iteration();
    void drawParticle();
};
const int nParticle = 3;
const int nCoordinates = 3;
Swarm s1 = Swarm(nCoordinates, nParticle);
void drawFunc(double (*)(double), double, double);
void display();
void timer(int);
int cnt =0;

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(0));
    
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500, 300);
    glutCreateWindow("Рой");
    
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(-M_PI, M_PI, -2, 2, 0, 1);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glutDisplayFunc(display);
//    glutIdleFunc(display);
    glutTimerFunc(10, timer, 0);
    glutMainLoop();
    
    return 0;
}

void plot(double (*f) (double),double a, double b)
{
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (double i = a; i < b; i += 0.01)
        glVertex2d(i, f(i));
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2d(0,-2);
    glVertex2d(0,2);
    glVertex2d(a, 0);
    glVertex2d(b, 0);
    glEnd();
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 21845);
    glBegin(GL_LINES);
    glVertex2d(M_PI/2,-2);
    glVertex2d(M_PI/2,2);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    plot(sin, -M_PI, M_PI);
    //s1.drawParticle();
    glColor3b(115, 0, 115);
    s1.getGreatBest().draw();
    s1.iteration();
    cnt++;
    glutSwapBuffers();
};
void timer(int A)
{
    glutPostRedisplay();
    if (d(s1.getGreatBest())>pow(10,-6))
        glutTimerFunc(200, timer, A++);
}
Swarm::Swarm(vector<Particle> TMP){
    Part = TMP;
    nIteration = 0;
}
Swarm::Swarm(int nCoordinates, int nParticle){
    cout << "Создание роя: \n";
    for (int i=0; i<nParticle; i++){ //Создание роя
        vector<double> tmp(nCoordinates);
        for (int j=0;j<nCoordinates;j++){
            tmp.at(j) = (double)rand()/RAND_MAX*M_PI/2;
        }
        Part.push_back(*new Particle(tmp));
        best.push_back(*new Particle(tmp));
        deltaXPrevious.push_back(*new Particle(nCoordinates));
        cout<<"Частица №"<<i<<endl;
        Part[i].Show();
    }
    nIteration = 0;
}
Particle Swarm::greatBest(){
    return greatBestParticle;
}
void Swarm::iteration(){
    cout<<"Итерация №"<<nIteration+1<<endl;
    double K0 = 0.9,K1=1,K2=1;
    if (nIteration == 3)
        K0 = 0.6;
    if (nIteration == 6)
        K0 = 0.2;
    bool once = true;
    int nParticle = (int) Part.size();
    int nCoordinates = (int) Part.at(0).getCoordinates().size();
    vector<double> tmp(nCoordinates,0);
    Particle deltaX(tmp);
    Particle greatBestParticle(tmp);
    for (int i = 0; i<nParticle; i++){
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
        }
    }
    for (int i=0; i<nParticle; i++) { //Particles
        Particle P(nParticle), q(nParticle);
        deltaX = (deltaXPrevious[i] * K0) + (P*(best[i]-Part[i]))*K1 + (q*(greatBestParticle - Part[i]))*K2;
        Part[i] = Part[i] + deltaX;
        deltaXPrevious[i] = deltaX;
        for (int j = 0; j < Part[i].getCoordinates().size(); j++) {
            if (Part[i].getCoordinate(j) < 0 ){
                Part[i].putCoordinate(j, abs(Part[i].getCoordinate(j)));
            };
            if (Part[i].getCoordinate(j) > M_PI/2 ){
                Part[i].putCoordinate(j, Part[i].getCoordinate(j)-M_PI/2);
            };
        }
        if (d(Part[i]) < d(best[i])){
            best[i] = Part[i];
            cout<<"Смена лучшего"<<endl;
        }
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
            cout<<"Смена самого лучшего"<<endl;
        }
        cout<<"Частица №"<<i<<": ";
        Part[i].Show();
        if ((i > 1)&&(Part[i-1] != Part[i]))
            once = false;
        cout<<"greatBest: ";
        greatBestParticle.Show();
        cout<<"Best №"<<i<<": ";
        best[i].Show();
        nIteration++;
        cout<<"D()="<<d(greatBestParticle)<<endl;
        cout<<endl;
    }
}

const bool operator<(Particle P, double p){
    for (int i=0; i<P.getCoordinates().size(); i++) {
        if (!(P.getCoordinate(i) < p)) {
            return false;
        }
    }
    return true;
}
const bool operator>(Particle P, double p){
    for (int i=0; i<P.getCoordinates().size(); i++) {
        if (!(P.getCoordinate(i) > p)) {
            return false;
        }
    }
    return true;
}
const bool operator== (Particle P, double p){
    return (P - p) < pow(10, -6);
}
const bool operator== (double p, Particle P){
    return P==p;
}
const bool operator== (Particle P1, Particle P2){
    bool tmp = true;
    for (int i=0;i<P1.getCoordinates().size();i++){
        if (P1.getCoordinate(i)!=P2.getCoordinate(i))
            tmp = false;
    }
    return tmp;
}
const bool operator!= (Particle P1, Particle P2){
    return !(P1==P2);
}
void Particle::putCoordinate(int n, double p){
    alpha.at(n) = p;
}
double Particle::getCoordinate(int n){
    return alpha.at(n);
}
vector<double> Particle::getCoordinates(){
    return alpha;
}
Particle::Particle(int nCoordinates){
    for (int i = 0; i < nCoordinates; i++)
        alpha.push_back((double)rand()/RAND_MAX*1.5);
}
Particle::Particle(){};
void Particle::Show(){
    sort(alpha.begin(), alpha.end());
    for (int i=0; i< alpha.size(); i++){
        cout<<alpha.at(i)<<" ";
    }
    cout<<endl;
}
Particle::Particle(vector<double> TMP){
    alpha = TMP;
}
Particle Particle::operator-(Particle P){
    unsigned long numberMax,numberMin;
    bool MaxMin;
    if (this->alpha.size() > P.alpha.size()){
        numberMax = this->alpha.size();
        numberMin = P.alpha.size();
        MaxMin = true;
    }
    else{
        numberMin = this->alpha.size();
        numberMax = P.alpha.size();
        MaxMin = false;
    }
    vector<double> result(numberMax,0.0);
    
    for (int i = 0; i<numberMin; i++){
        result[i]=alpha[i]-P.alpha[i];
    };
    return Particle(result);
}
Particle Particle::operator*(int p){
    vector<double> result(alpha.size());
    for (int i=0; i<alpha.size(); i++){
        result[i]=alpha[i] * p;
    }
    return Particle(result);
}
Particle Particle::operator*(Particle P){
    unsigned long numberMax,numberMin;
    bool MaxMin;
    if (this->alpha.size() > P.alpha.size()){
        numberMax = this->alpha.size();
        numberMin = P.alpha.size();
        MaxMin = true;
    }
    else{
        numberMin = this->alpha.size();
        numberMax = P.alpha.size();
        MaxMin = false;
    }
    vector<double> result(numberMax,0.0);
    
    for (int i = 0; i<numberMin; i++){
        result[i] = alpha.at(i)*P.alpha.at(i);
    };
    
    return Particle(result);
}
Particle Particle::operator+(Particle P){
    unsigned long numberMax,numberMin;
    bool MaxMin;
    if (this->alpha.size() > P.alpha.size()){
        numberMax = this->alpha.size();
        numberMin = P.alpha.size();
        MaxMin = true;
    }
    else{
        numberMin = this->alpha.size();
        numberMax = P.alpha.size();
        MaxMin = false;
    }
    vector<double> result(numberMax,0.0);
    
    for (int i = 0; i<numberMin; i++){
        result[i] = alpha[i]-P.alpha[i];
    };
    return Particle(result);
};
double d(Particle P){
    double tmp = 0.0;
    for (int i=1; i<=P.getCoordinates().size(); i++){
        double tmp1 = 0.0;
        for (int j=1; j<=P.getCoordinates().size(); j++){
            tmp1 += cos((2*i+1) * P.getCoordinate(j-1));
        }
        tmp += (tmp1/(2*i+1))*(tmp1/(2*i+1));
    }
    return tmp;
}
void Swarm::drawParticle(){
    for (int i = 0; i<Part.size(); i++){
        glColor3b(i, i, i);
        Part.at(i).draw();
    }
}
void Particle::draw(){
    for(int i=0;i<alpha.size();i++)
    {
        glColor3f(0.0,1.0,1.0);
        glBegin(GL_LINE_STRIP);
        glVertex2d(alpha.at(i),sin(alpha.at(i)));
        glVertex2d(alpha.at(i), 0);
        glEnd();
        
    }
}

