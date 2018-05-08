//
//  main.cpp
//  particleSwarmMetodForSin
//
//  Created by Артем on 01.05.2018.
//  Copyright © 2018 Артем. All rights reserved.
//

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
    Particle operator+(Particle P);
    Particle operator*(Particle P);
    Particle operator*(int p);
    Particle operator-(Particle P);
//    Particle operator=(Particle P);
    void Show();
};
double Particle::getCoordinate(int n){
         return alpha.at(n);
}
vector<double> Particle::getCoordinates(){
    return alpha;
}
Particle::Particle(int nCoordinates){
    for (int i = 0; i < nCoordinates; i++)
        alpha.push_back(rand()% 15707/10000.0);
}
Particle::Particle(){};
void Particle::Show(){
    for (int i=0; i< alpha.size(); i++){
        cout<<alpha.at(i)<<" ";
    }
    cout<<endl;
}
Particle::Particle(vector<double> TMP){
    alpha = TMP;
}
//Particle Particle::operator=(Particle P){
//    for (int i=0; i< alpha.size(); i++)
//        alpha[i] = P.alpha[i];
//    return *this;
//}
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
       result[i] = alpha[i] + P.alpha[i];
    };
    return Particle(result);
};
double d(Particle P){
    double tmp = 0.0;
    for (int i=0; i<P.getCoordinates().size(); i++)
    {
        double tmp1 = 0.0;
        for (int j=1; j<=P.getCoordinates().size(); j++){
            tmp1 += cos((2*j+1) * P.getCoordinate(j-1));
        }
        tmp += tmp1*tmp1;
    }
    return tmp;
}
int main(int argc, const char * argv[]) {
    const int nParticle = 7;
    const int nCoordinates = 7;
    srand((unsigned int)time(0));
    cout << "Создание роя: \n";
    vector<Particle> Part(nParticle);
    vector<Particle> best(nParticle);
    vector<Particle> deltaXPrevious(nParticle);
    for (int i=0; i< nParticle; i++){
        vector<double> tmp(nCoordinates);
        for (int j=0;j<nCoordinates;j++)
            tmp.at(j) = rand()% 15707/10000.0;
        Part[i] = *new Particle(tmp);
        best[i] = Part[i];
        deltaXPrevious[i] = *new Particle(nCoordinates);
        Part[i].Show();
    }
    double K0 = 0.9;
    const double K1 = 1, K2(1);
    vector<double> tmp(nCoordinates,0);
    Particle deltaX(tmp);
    Particle greatBestParticle(tmp);
    for (int i = 0; i<nParticle; i++){
        if (d(Part[i]) < d(greatBestParticle)){
            greatBestParticle = Part[i];
        }
    }
    
    int nIteration = 0;
    do {
        cout<<"Итерация №"<<nIteration+1<<endl;
        for (int i=0; i<nParticle; i++) { //Particles
            Particle P(nParticle), q(nParticle);
            deltaX = (deltaXPrevious[i] * K0) + (P*(best[i]-Part[i]))*K1 + (q*(greatBestParticle - Part[i]))*K2;
            Part[i] = Part[i] + deltaX;
            deltaXPrevious[i] = deltaX;
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
            cout<<"greatBest: ";
            greatBestParticle.Show();
            cout<<"Best №"<<i<<": ";
            best[i].Show();
            cout<<endl;
        }
        nIteration++;
        if (nIteration == 3)
            K0 = 0.6;
        if (nIteration == 6)
            K0 = 0.2;
        cout<<"D "<<d(greatBestParticle)<<endl;
    } while (d(greatBestParticle)>pow(10, -12)); //iteration
    cout<<"Результат: ";
    greatBestParticle.Show();
    
    return 0;
}
