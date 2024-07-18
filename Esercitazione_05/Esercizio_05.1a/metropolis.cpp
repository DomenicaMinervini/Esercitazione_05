/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

//SCRIVO L'ALGORITMO DI METROPOLIS

#include "random.h"

using namespace std;

double radius(double x, double y, double z){
   return sqrt(x*x+y*y+z*z);
}

double rapporto_probabilita_ground_state(double r_old, double r_new){
   return exp(-2.0*r_new)/exp(-2.0*r_old);
}

double rapporto_probabilita_stato_eccitato(double r_old, double r_new){
   return (exp(-r_new)/exp(-r_old))*((r_new*r_new)/(r_old*r_old));
}

double minimum(double rapp){
   if(rapp<1.0) return rapp;
   else return 1.0;
}

int main (int argc, char *argv[]){

   //generatore di numeri casuali

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   //inizia parte sull'algoritmo di Metropolis

   /*Ho fatto il test sul valore ideale del range in cui estrarre i valori casuali, in unita' di raggio di Bohr la percentuale
   ideale di valori accettati deve essere del 50%. Ho fatto 10000 estrazioni. Il valore del range che si avvicina di piu' a
   questa percentuale e' 1.237. Da come percentuale di accettazione 50.09% nel caso dello stato fondamentale. 
   Per lo stato eccitato invece la percentuale di accettazione e' del 50.12%, per range=4.36.*/

   double delta_x, delta_y, delta_z; //spostamento infinitesimo dalla posizione attuale
   double x_old, y_old, z_old; //coodinate in cui mi trovo adesso, attuali
   double x_new, y_new, z_new; //coordinate successive

   double r_old, r_new; //posizione attuale e successiva
   double rapp, alpha, value; //rapporto probabilita', probabilita' scelta, valore tra 0 e 1

   int i, lanci=10000; //contatore e numero di valori generati
   int accettato=0; //ripetuto=0; numero mosse accettate e numero di mosse ripetute
   double percentuale; //percentuale di accettazione di Metropolis

   double totale=0; //sommo tutti i raggi
   double media; //faccio la media
   
   range = atof(argv[1]);
   cout << range << endl;
   
   //posizione iniziale
   /*x_old=0.866;
   y_old=0.866;
   z_old=0.866;*/

   x_old=2.887;
   y_old=2.887;
   z_old=2.887;

   for(i=0; i<lanci; i++){

      //mi sposto in un intorno tra la mia posizione attuale e il range:
      //e' il raggio che da come probabilita' di accettazione circa 0.5

      delta_x=rnd.Rannyu(-range,range);
      delta_y=rnd.Rannyu(-range,range);
      delta_z=rnd.Rannyu(-range,range);

      x_new=x_old+delta_x;
      y_new=y_old+delta_y;
      z_new=z_old+delta_z;

      //calcolo i raggi: sono in tre dimensioni
      r_old=radius(x_old,y_old,z_old);
      r_new=radius(x_new,y_new,z_new);

      //cout << r_old << "  " << r_new << endl;

      //calcolo il rapporto tra la probabilita nuova e la vecchia
      //rapp=rapporto_probabilita_ground_state(r_old,r_new);
      rapp=rapporto_probabilita_stato_eccitato(r_old, r_new);

      //scelgo il minimo tra 1 e il rapporto tra le probabilita': in questo modo 
      //ho accettato con probabilita' alpha la mossa da r_old a r_new
      alpha=minimum(rapp);

      //APPLICO L'ALGORITMO DI METROPOLIS
      //estraggo un valore in una distribuzione uniforme tra 0 e 1
      //se alpha e' maggiore del valore estratto, accetto la mossa nuova
      //altrimenti tengo la posizione in cui mi trovo
      value=rnd.Rannyu();

      if(value<=alpha){ //accetto la mossa: la posizione nuova diventa quella attuale
         x_old=x_new;
         y_old=y_new;
         z_old=z_new;  
         accettato++;
      }

      totale += r_old;
   }

   media = totale/lanci;
   percentuale = 100.0*double(accettato)/double(lanci);
   cout << "Media dei raggi: " << media << endl;
   cout << "Percentuale accettazione: " << percentuale << "%" << endl;

   rnd.SaveSeed();
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
