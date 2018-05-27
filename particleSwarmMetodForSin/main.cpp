#include <iostream>
#include <vector>
#include <cmath>
#include <GLUT/GLUT.h>
#include <time.h>
#include <unistd.h>

#define eps pow(10,-6)
const int nParticle = 50 ;
const int nCoordinates =3 ;
using namespace std;
class Particle{
private:
    vector<long double> alpha;
public:
    Particle(vector<long double>);
    Particle(int);
    Particle();
    vector<long double> getCoordinates();
    long double getCoordinate(int);
    void putCoordinate (int, long double);
    Particle operator+(Particle P);
    Particle operator*(Particle P);
    Particle operator*(int p);
    Particle operator-(Particle P);
    void Show();
    void draw();
    bool equality();
};
class Swarm{
private:
    vector<Particle> Part,best,deltaXPrevious;
    Particle greatBestParticle;
    int nIteration;
    int nSmena, nNullDelta;
    long double K0, K1, K2;
public:
    Swarm(int nCoordinates, int nParticle);
    int iteration();
    Particle getGreatBest(){return greatBestParticle;};
    int getNIteration(){return nIteration;};
};
const bool operator== (Particle P, long double p);
const bool operator== (long double p, Particle P);
const bool operator== (Particle P1, Particle P2);
const bool operator!= (Particle P1, Particle P2);
const bool operator< (Particle P1, long double p);
const bool operator> (Particle P1, long double p);
long double d(Particle P);
void display();
void timer(int);
void plot(long double(*) (long double), long double, long double);
Swarm s1(nCoordinates, nParticle);
int main(int argc, const char * argv[]) {
    srand(time(0));
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(700, 300);
    glutCreateWindow("Рой");
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(-M_PI/2, M_PI, -2, 2, 0, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glutDisplayFunc(display);
    glutTimerFunc(1, timer, 0);
    glutMainLoop();
    return 0;
}
void plot(long double (*f) (long double),long double a, long double b){
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (long double i = a; i < b; i += 0.01)
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
    s1.getGreatBest().draw();
    if (d(s1.getGreatBest())>eps){
        if (s1.iteration() == 10){
            s1 = Swarm(nCoordinates,nParticle);
        }
    }else{
        cout<<"Результат :"<<s1.getNIteration()<<" ";
        s1.getGreatBest().Show();
        getchar();
    }
    glutSwapBuffers();
};
void timer(int A)
{
    glutPostRedisplay();
    if (d(s1.getGreatBest())>eps)
        glutTimerFunc(20, timer, 0);
}
Swarm::Swarm(int nCoordinates, int nParticle){
    cout << "Создание роя: \n";
    for (int i=0; i< nParticle; i++){
        vector<long double> tmp(nCoordinates);
        for (int j=0;j<nCoordinates;j++){
            
            tmp.at(j) =(long double)rand()/RAND_MAX*M_PI/2;
        }
        Part.push_back(*new Particle(tmp));
        best.push_back(Part[i]);
        deltaXPrevious.push_back(*new Particle(nCoordinates));
        Part[i].Show();
    }
    greatBestParticle = Part[0];
    for (int i = 0; i<nParticle; i++){
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
        }
    }
    cout<<"GreatBest "<<endl;
    greatBestParticle.Show();
    cout<<endl;
    nIteration = 0;
    nSmena = 0;
    nNullDelta = 0;
    K0 = 0.8; K1 = 1.0; K2 = 1.0;
}
int Swarm::iteration(){
    vector<long double> tmp(nCoordinates,0);
    Particle deltaX(tmp);
    cout<<"Итерация № \007"<<nIteration+1<<endl;
    for (int i=0; i<nParticle; i++) { //Particles
        Particle P(nCoordinates), q(nCoordinates);
        deltaX = (deltaXPrevious[i] * K0) + (P*(best[i]-Part[i])) * K1+(q*(greatBestParticle - Part[i]))*K2;
        if (deltaX == 0.0){
            nNullDelta++;
        }
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
            nSmena = 0;
        }
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
            cout<<"Смена самого лучшего"<<endl;
            nSmena = 0;
        }
        cout<<"Частица №"<<i<<": ";
        Part[i].Show();
        cout<<"greatBest: ";
        greatBestParticle.Show();
        cout<<"Best №"<<i<<": ";
        best[i].Show();
        cout<<"D(Great) "<<d(greatBestParticle)<<endl;
        cout<<endl;
        if (Part[i].equality()){
            s1 = Swarm(nCoordinates, nParticle);
            return 0;
        }
    }
    nIteration++;
    nSmena++;
    if (d(greatBestParticle)<pow(10,-4 )) {
        K0 = 0.01;
    }
    if (d(greatBestParticle)< 5* pow(10,-6)) {
        K0 = 0.001;
    }
    cout<<"D(Great) "<<d(greatBestParticle)<<endl;
    cout<<"greatBest: ";
    greatBestParticle.Show();
    if ((nSmena>=5*nCoordinates)&&(nNullDelta >= nCoordinates)){
        printf("\e[1;1H\e[2J");
        system("clear");
        cout<<"clear"<<endl<<endl;
        return 10;
        
    }
    return 0;
}
const bool operator<(Particle P, long double p){
    for (int i=0; i<P.getCoordinates().size(); i++) {
        if (!(P.getCoordinate(i) < p)) {
            return false;
        }
    }
    return true;
}
const bool operator>(Particle P, long double p){
    for (int i=0; i<P.getCoordinates().size(); i++) {
        if (!(P.getCoordinate(i) > p)) {
            return false;
        }
    }
    return true;
}
const bool operator== (Particle P, long double p){
    return (P - p) < pow(10, -6);
}
const bool operator== (long double p, Particle P){
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
void Particle::putCoordinate(int n, long double p){
    alpha.at(n) = p;
}
long double Particle::getCoordinate(int n){
    return alpha.at(n);
}
vector<long double> Particle::getCoordinates(){
    return alpha;
}
Particle::Particle(int nCoordinates){
    for (int i = 0; i < nCoordinates; i++)
        alpha.push_back((long double)rand()/RAND_MAX*1.5);
}
Particle::Particle(){};
void Particle::Show(){
    sort(alpha.begin(), alpha.end());
    for (int i=0; i< alpha.size(); i++){
        cout<<alpha.at(i)<<" ";
    }
    cout<<endl;
}
Particle::Particle(vector<long double> TMP){
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
    vector<long double> result(numberMax,0.0);
    
    for (int i = 0; i<numberMin; i++){
        result[i]=alpha[i]-P.alpha[i];
    };
    return Particle(result);
}
Particle Particle::operator*(int p){
    vector<long double> result(alpha.size());
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
    vector<long double> result(numberMax,0.0);
    
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
    vector<long double> result(numberMax,0.0);
    
    for (int i = 0; i<numberMin; i++){
        result[i] = alpha[i]-P.alpha[i];
    };
    return Particle(result);
};
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
long double d(Particle P){
    
    long double tmp = 0.0;
        for (int i=1; i<=P.getCoordinates().size(); i++){
            long double tmp1 = 0.0;
            for (int j=1; j<=P.getCoordinates().size(); j++){
                tmp1 += cos((2*i+1) * P.getCoordinate(j-1));
            }
            tmp += tmp1*tmp1/((2*i+1)*(2*i+1));
    }
    return tmp;
}
bool Particle::equality(){
    for (int i=1; i<alpha.size(); i++) {
        if (abs(alpha.at(i-1) - alpha.at(i))<pow(10, -5)){
            return true;
        }
    }
    return false;
}
