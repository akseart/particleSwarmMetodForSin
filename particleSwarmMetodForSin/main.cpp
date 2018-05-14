#include <iostream>
#include <vector>
#include <cmath>
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
};
const bool operator== (Particle P, double p);
const bool operator== (double p, Particle P);
const bool operator== (Particle P1, Particle P2);
const bool operator!= (Particle P1, Particle P2);
const bool operator< (Particle P1, double p);
const bool operator> (Particle P1, double p);
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
int main(int argc, const char * argv[]) {
    const int nParticle = 50 ;
    const int nCoordinates = 7;
    srand((unsigned int)time(0));
begin:
    cout << "Создание роя: \n";
    vector<Particle> Part(nParticle);
    vector<Particle> best(nParticle);
    vector<Particle> deltaXPrevious(nParticle);
    for (int i=0; i< nParticle; i++){
        vector<double> tmp(nCoordinates);
        for (int j=0;j<nCoordinates;j++){
            
            tmp.at(j) =(double)rand()/RAND_MAX*M_PI/2;
        }
        Part[i] = *new Particle(tmp);
        best[i] = Part[i];
        deltaXPrevious[i] = *new Particle(nCoordinates);
        Part[i].Show();
    }
    cout<<endl;
    double K0 = 0.4;
    const double K1 = 1.5, K2(1.5);
    vector<double> tmp(nCoordinates,0);
    Particle deltaX(tmp);
    Particle greatBestParticle(Part[0]);
    for (int i = 0; i<nParticle; i++){
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
        }
    }
    cout<<"GreatBest "<<endl;
    greatBestParticle.Show();
    int nIteration = 0;
    int nSmena = 0, nNullDelta = 0;
     bool once = true;
    do {
        cout<<"Итерация №"<<nIteration+1<<endl;
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
            if ((i > 1)&&(Part[i-1] != Part[i]))
                once = false;
            cout<<"greatBest: ";
            greatBestParticle.Show();
            cout<<"Best №"<<i<<": ";
            best[i].Show();
            cout<<"D(Great) "<<d(greatBestParticle)<<endl;
            cout<<endl;
            
        }
        nIteration++;
        nSmena++;
        if (nIteration == 3)
            K0 = 0.6;
        if (nIteration == 6)
            K0 = 0.9;
//        if (nIteration == 10)
//            K0 = 1.1;
        cout<<"D(Great) "<<d(greatBestParticle)<<endl;
        if ((nSmena>=5*nCoordinates)&&(nNullDelta >= nCoordinates))
            break;
    } while (d(greatBestParticle)>pow(10, -6)); //iteration
    if ((nSmena>=5*nCoordinates)&&(nNullDelta >= 5*nCoordinates)){
        cout<<"Частицы сошлись воедино\nSorry Начать сначала";
//        getchar();
        goto begin;
    }
   
    cout<<"Результат: "<<nIteration<<" ";
    greatBestParticle.Show();
    
    return 0;
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
