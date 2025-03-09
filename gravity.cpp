#include <iostream>
#include <cmath>

const int G = 1;

class vektor {
public:
    double x,y;
    vektor(double x = 0, double y = 0) : x(x), y(y) {}
    vektor topla(vektor v) const{ return vektor(x + v.x, y + v.y); }
    vektor cikar(vektor v) const{ return vektor(x - v.x, y - v.y); }
    vektor carp(double scalar) const{ return vektor(x * scalar, y * scalar); }
    vektor bol(double scalar) const{ return vektor(x / scalar, y / scalar); }

    //vektor operator+(const vektor& v) const { return vektor(x + v.x, y + v.y); }
    //vektor operator-(const vektor& v) const { return vektor(x - v.x, y - v.y); }
    //vektor operator*(double scalar) const { return vektor(x * scalar, y * scalar); }
    //vektor operator/(double scalar) const { return vektor(x / scalar, y / scalar); }
    double buyukluk() const { return sqrt(x * x + y * y); }

};

class cisimler {
public:
    double kutle;
    vektor konum, hiz;

    cisimler(double kutle, vektor konum, vektor hiz) : kutle(kutle), konum(konum), hiz(hiz) {}
    vektor ivme;

    void guncelle(vektor kuvvet) {
        ivme = kuvvet.bol(kutle);
        //ivme = kuvvet / kutle;
        hiz = hiz.topla(ivme);
        //hiz = hiz + ivme;
        konum = konum.topla(hiz);
        //konum = konum + hiz;
    }
};

class rocket : public cisimler {
public:
    double puskurtmeHizi, puskurtmeOrani,yakitMiktari;
    rocket(double m, vektor xy, vektor hiz, double puskurtmeHizi, double puskurtmeOrani, double yakitMiktari);

    void guncelle(vektor kuvvet);
};
rocket::rocket(double m, vektor xy, vektor hiz, double puskurtmeHizi, double puskurtmeOrani, double yakitMiktari) : cisimler(m, xy, hiz), puskurtmeHizi(puskurtmeHizi), puskurtmeOrani(puskurtmeOrani), yakitMiktari(yakitMiktari) {}
void rocket::guncelle(vektor kuvvet) {}

class  node{
public:
    cisimler* cisim;
    node* sonraki;
    node(cisimler* c) : cisim(c), sonraki(nullptr) {}
};

class liste {
public:
    node* bas;
    liste() : bas(nullptr) {}
    void ekle(cisimler* c);
};
void liste::ekle(cisimler* c) {}

class karsilastirma {
public:
    liste cisimler;
    void cisimEkle();
    void ekle();
    void calistir();
    void konumHesapla();

};

void karsilastirma::calistir() {}
