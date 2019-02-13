#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avalam.h>
#include <topologie.h>

//PERMET DE DEMANDER QUEL COUP VA ETRE JOUER
void selection( int *arr, int *dest )
{
                printf0("Demande du coup à jouer...\n");

                printf("Cage d'origine ?\n");
                scanf("%d", arr);
                
                printf("Case de destination ?\n");
                scanf("%d", dest);

                printf("vous avez choisi de deplacer la case %d sur la case %d\n", (*arr), (*dest) );

}

//Enregistre la position dans le fichier json
void enregistrer(T_Position p, const char *ch){

   T_Score s;
   FILE * fp;
   
   char ch2[50] = "../web/data/";
   
        s = evaluerScore(p);
        afficherScore(s);

        strcat(ch2, ch);

        printf1("enregistrement dans %s...\n", ch2);

        printf0("Ouverture du fichier json...\n");
    
    fp = fopen (ch2, "w+");

    if (fp==NULL)//Si le fichier n'existe pas affichage d'erreur
    { 
        printf("no such file.");
    } 

    //Modification du fichier Json
    printf0("Modification du fichier Json...\n");
    fprintf(fp, "traiterJson({\n");
    fprintf(fp, "\"trait\":%d,\n", p.trait );
    fprintf(fp, "\"scoreJ\":%d,\n", s.nbJ );
    fprintf(fp, "\"scoreJ5\":%d,\n", s.nbJ5 );
    fprintf(fp, "\"scoreR\":%d,\n", s.nbR );
    fprintf(fp, "\"scoreR5\":%d,\n", s.nbR5 );
    fprintf(fp, "\"cols\":[\n");
   
   for(int i = 0; i < 47; i++){
           fprintf(fp, "{\"nb\":%d, \"couleur\":%d},\n", p.cols[i].nb, p.cols[i].couleur );
   }
   fprintf(fp, "{\"nb\":%d, \"couleur\":%d}\n", p.cols[47].nb, p.cols[47].couleur );
   
   
   fprintf(fp, "]\n");
   fprintf(fp, "});");
   
   
    fclose(fp);
    printf0("Fermeture du fichier Json...\n");

}


//----------------------------MAIN----------------------------
//Pour visualiser en même temps le jeu sur une page web avec "Avalam-refresh.html" : 
//lier le fichier "data/default.js" (ou celui que vous passez en argument) dans le champ en bas de la page

//ACTIVATION DU MODE DEBUG : ????????

//Possibilité de passer en ligne de commande le nom d'un fichier ".js" existant ou a créer 
//Sinon il y a un fichier par défaut : "data/default.js"
int main( int argc, char *argv[]) 
{

	T_Position p; 
	T_ListeCoups l; 
    int arr;//prend la case de départ
    int dest;//case de destination
    int num;//Pour le mode debug
    T_Score score;//permet de recevoir le score

	printf0("Création de la position initiale ...\n"); 
	p = getPositionInitiale();
	
	printf0("Récupération des coups légaux de la position initiale ...\n"); 
 	l = getCoupsLegaux(p);

	printf("Depuis la position initiale du jeu, le trait est aux %ss\n", COLNAME(p.trait));


        while( l.nb != 0)//Fin de partie quand on sort du while
        {

        printf1("Il y a %d coups possibles...\n", l.nb);

        printf1("Entree dans le while pour la %d fois...\n",&num);
        num=num+1;

        //Donne qui doit jouer        
        printf("\nle trait est aux %ss\n",COLNAME(p.trait));
        
        //Demande le coup à jouer
        selection(&arr, &dest);

        printf0("Verification de la validite du coup...\n");

            //Vérifie que le coup est possible et l'enregistrer dans un fichier par défaut si aucun fichier n'est spécifié      
            if ( estValide( p , arr, dest ) == 1 )
            { 
                    printf("Le coup est possible\n");
                    p = jouerCoup( p , arr, dest);
                    
                    printf1("argc = %d\n", argc);
                    
                    if( argc == 1 )
                    {
                        printf0("Aucun fichier specifie en ligne de commande = enregistrement par defaut...\n");
                        enregistrer(p, "default.js");
                    }
                  
                    if( argc > 1 )
                    {
                        printf0("Enregistrement dans le fichier de la ligne de commande...\n");
                        enregistrer(p, argv[1]);
                    }

                    printf0("Affichage de la position actuelle des pions...\n");
                    afficherPosition0(p);
            }

            //Récupération des coups légaux
            printf0("Récupération des coups légaux de la position initiale ...\n"); 
            l = getCoupsLegaux(p);
        
        }

        //Fin de partie
        printf("PARTIE FINIE !!!!\n");

        //evaluer score position p
        printf0("Evaluation du score...\n");
        score=evaluerScore(p);

        //interpréter le score
        printf0("Interpretation du score...\n");
        if (score.nbJ>score.nbR)//Cas où Jaunes gagnent
        {
            printf("Les Jaunes ont gagnés\n");
        }
        if (score.nbJ<score.nbR)//Cas où Rouges gagnent
        {
            printf("Les Rouges ont gagnés\n");
        }
        if (score.nbJ==score.nbR)//si égalité sur les tours, on comparent les tours de 5
        {
            if (score.nbJ5>score.nbR5)//Cas où Jaunes gagnent
            {
                printf("Les Jaunes ont gagnés\n");
            }
            if (score.nbJ5<score.nbR5)//Cas où Rouges gagnent
            {
                printf("Les Rouges ont gagnés\n");
            }
            if (score.nbJ5==score.nbR5)//Cas où Rouges gagnent
            {
                printf("Egalités !!!\n");
            }
        }



	return 0;
}
