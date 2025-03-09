#include "gravity.cpp"
#include "canvas (2).cpp"


int main()
{
canvas graphic("gravity");
graphic.startDoc();
graphic.drawFrame();

cisimler m1(150, vektor(400,0), vektor(0,0.2));
cisimler m2(70, vektor(0,300), vektor(-0.15,0));
cisimler m3(150, vektor(-200,-200), vektor(0.1,-0.1));

karsilastirma U;
U.cisimEkle(&m1);
U.cisimEkle(&m2);
U.cisimEkle(&m3);

for(int t=0; t<2000; t++)
{
  U.run(timestep*100); // run simulation for a while and draw last position
  graphic.drawPoint(U.getPosition(1).x, U.getPosition(1).y,"red");
  graphic.drawPoint(U.getPosition(2).x, U.getPosition(2).y,"green");
  graphic.drawPoint(U.getPosition(3).x, U.getPosition(3).y,"blue");
 }

graphic.finishDoc();
 return 0;
}
