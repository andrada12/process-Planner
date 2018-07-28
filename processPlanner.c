#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct proces{

	char nume[21];
	int timp;
	int prioritate;
	struct proces *next;
}proces;

/*aceasta functie preia informatiile citite din fisier si 
le pune in strunctura. Se departe sirul citit, caci fisierul
se citeste linie cu linie, printr-un string, iar acest string
e adaugat caracter cu caracter in structura. Functia se utilizeaza
pentru functia add, cand strignul poate fi impartit in  functie
de spatiile din el. Functia adauga la sfarsitul cozii. */

proces *add(proces *coada, char s[]){
	proces *nou=(proces *)calloc(1, sizeof(proces));
	int i=2,k=0;
	nou->next=NULL;

	while(s[i]!=' '){
 	nou->nume[k]=s[i];
 	k++;
 	i++;
 }
 	i++; nou->timp=0;
 	while(s[i]!=' '){
 		nou->timp=nou->timp*10+((int)s[i]-48);
 		i++;
 	}

 	i++;
 	nou->prioritate=0;
 	while(i<strlen(s)-1){
 		nou->prioritate=nou->prioritate*10+((int)s[i]-48);
 		i++;
 	}

 	if(coada==NULL)
 		return nou;

 	proces *p=coada;
 	while(p->next!=NULL)
 		p=p->next;
 	p->next=nou;

 	return coada;

}

/* aceasta functie este folosita pentru a muta un proces dintr-o
coada in alta. Primul argument dat este numele cozii in care se
muta procesul, iar urmatoarele 3, sunt componentele procesului
mutat(numele,, timpul si prioritatea). Functia adauga la sfarsitul
cozii. */
proces *add2(proces *coada, char s[], int t, int p)
{
	proces *nou=(proces *)calloc(1,sizeof(proces));
	int i=0;

	while(i<strlen(s)){
		nou->nume[i]=s[i];
		i++;
	}

	nou->timp=t;
	nou->prioritate=p;
	nou->next=NULL;

	if(coada == NULL)
		return nou;

	proces *q=coada;
	while(q->next !=NULL)
		q=q->next;
	q->next=nou;

	return coada;
}

/*Functia este apelata in cadrul functiei pentru multiple-add.
Stringul s este un proces delimitat din sirul preluat din fiser,
ce contine mai mmulte procese. Functia functioneaza pe acelasi 
principiu ca add, mutand in coada informatia din string*/
proces *add_ma(proces *coada, char s[]){
	proces *nou=(proces *)calloc(1, sizeof(proces));
	int i=0,k=0;
	nou->next=NULL;

	while(s[i]!=' '){
 	nou->nume[k]=s[i];
 	k++;
 	i++;
 }
 	i++; nou->timp=0;
 	while(s[i]!=' '){
 		nou->timp=nou->timp*10+((int)s[i]-48);
 		i++;
 	}

 	i++;
 	nou->prioritate=0;
 	while(i<strlen(s)){
 		nou->prioritate=nou->prioritate*10+((int)s[i]-48);
 		i++;
 	}

 	if(coada==NULL)
 		return nou;

 	proces *p=coada;
 	while(p->next!=NULL)
 		p=p->next;
 	p->next=nou;

 	return coada;

}


/*Functia aceasta este apelata atunci cand programul primeste
comanda 'ma'. Sirul citit din fisier, care contine mai multe 
procese, este impartit intr-un string care contine doar un 
singur proces, care este mai apoi pus in coada, prin 
functia add_ma*/
proces *multiple_add(proces *coada, char s[]){

	int i=3;
	int k,spatii;
	while(i<strlen(s)){
		char aux[21]="\0";
		k=0;
		spatii=0;
		while(spatii<3 && i<strlen(s)){
			if(s[i]== ' ')
				spatii++;
			aux[k]=s[i];
			k++; i++;

		}
		if(aux[strlen(aux)-1]==' ')
			aux[strlen(aux)-1]='\0';
		if(aux[strlen(aux)-1]=='\n')
			aux[strlen(aux)-1]='\0';
		coada=add_ma(coada,aux);
	}
	return coada;
	
}

/*Prin aceasta functie sterg primul element dintr-o coada*/
proces *del(proces *coada){

	if(coada==NULL)
		return NULL;

	proces *p=coada;
	coada=coada->next;
	free(p);
	return coada;
}

/*Aceasta functie micsoreaza timpul unui proces, la primirea
comenzii tick*/
proces *tick(proces *coada)
{
	if(coada->timp>0)
	coada->timp=coada->timp-1;
	return coada;
}

/*Functia del2 sterge un anumit proces dintr-o coada,
proces identificat dupa numele sau. Prin 's' este identificat
procesul ce trebuie sters. Este folosita la trezirea unui proces
din WAITING*/
proces *del2(proces *coada,char s[]){

	if(strcmp(coada->nume,s)==0){
		proces *q=coada;
		coada=coada->next;
		free(q);
	}
  else{
	proces *q=coada;
	while(strcmp(q->next->nume,s)!=0)
		q=q->next;
	proces *aux=q->next;
	q->next=aux->next;
	free(aux);
}
	return coada;

}

/*aceasta functie alege procesul cu cel mai scurt timp,
intorcand un pointer la acest prces*/
proces *alegegere_SJF(proces *coada){
	int min=coada->timp;
	proces *q=coada;
	while(q!=NULL){
		if(q->timp<min)
			min=q->timp;
	q=q->next;
	}
	q=coada;
	while(q->timp!=min)
		q=q->next;

	return q;
}

/*aceasta functie alege procesul cu cea mai mare prioritate,
intorcand un pointer la acest prces*/
proces *alegere_PP(proces *coada){
	int max=coada->prioritate;
	proces *q=coada;
	while(q!=NULL){
		if(q->prioritate>max)
			max=q->prioritate;
		q=q->next;
	}
	q=coada;
	while(q->prioritate!=max)
		q=q->next;
	return q;

}


int main(int argc, char const *argv[])
{
	int proces_ales,cuanta=0,timp_petrecut=0;
	char s[80];
	proces *READY=NULL, *RUNNING=NULL, *WAITING=NULL, *MIN=NULL, *MAX=NULL, *AUX_MAX=NULL;
	FILE *f;
	FILE *f2;
	f=fopen(argv[1],"r");
	f2=fopen(argv[2],"w");
	if(f==NULL){ 
	printf("Nu se poate deschide la citire fişierul!\n");

		exit(1); 
}
	while(fgets(s,80,f)){

		if(s[0]>=48 && s[0]<=57){
			proces_ales=(int)s[0]-48;
			if(proces_ales == 3 || proces_ales == 4)
				cuanta=(int)s[2]-48;
		}
		if(proces_ales == 1 || proces_ales == 2 || proces_ales == 3){
		if(s[0]=='a'){
			READY=add(READY,s);
			if(RUNNING==NULL){
				if(READY!=NULL){
					if(proces_ales== 2){
						MIN=alegegere_SJF(READY);
						RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
						READY=del2(READY,MIN->nume);
					}
					else{
					RUNNING=add(RUNNING, s);
					READY=del(READY);
					if(proces_ales == 3)
						timp_petrecut=0;	
					}
				}
			}
			else{
				if(RUNNING!=NULL){
					RUNNING=tick(RUNNING);
					if(proces_ales == 3)
						timp_petrecut++;
					if(RUNNING->timp==0){
						RUNNING=del(RUNNING);
						if(READY != NULL){
							if(proces_ales== 2){
							MIN=alegegere_SJF(READY);
							RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
							READY=del2(READY,MIN->nume);
							}
							else{
								RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
								READY=del(READY);
								if(proces_ales == 3)
									timp_petrecut=0;
							}
						}
					}
					else{
						if(proces_ales == 3 && timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							timp_petrecut=0;
						}
					}

				}
			}
			
		}
		if(s[0]=='m'){
			READY=multiple_add(READY,s);
			//show(READY);
			if(RUNNING==NULL){
				if(READY!=NULL){
					if(proces_ales== 2){
						MIN=alegegere_SJF(READY);
						RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
						READY=del2(READY,MIN->nume);
					}
					else{
					RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
					READY=del(READY);	
					if(proces_ales == 3)
						timp_petrecut=0;
					}
				}
			}
			else{
				if(RUNNING!=NULL){
					RUNNING=tick(RUNNING);
					timp_petrecut++;
					if(RUNNING->timp==0){
						RUNNING=del(RUNNING);
						if(READY != NULL){
							if(proces_ales== 2){
							MIN=alegegere_SJF(READY);
							RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
							READY=del2(READY,MIN->nume);
							}
							else{
								RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
								READY=del(READY);
								if(proces_ales == 3)
									timp_petrecut=0;
							}
						}
					}
					else{
						if(proces_ales == 3 && timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							timp_petrecut=0;
						}
					}

				}
			}
			
		}
		if(s[0]=='s'){
			if(RUNNING == NULL)
				fprintf(f2, "\n");
			else
			fprintf(f2, "%s %d\n", RUNNING->nume, RUNNING->timp );
		}
		if(s[0]=='t'){
			if(RUNNING!=NULL){
				RUNNING=tick(RUNNING);
					timp_petrecut++;   
				if(RUNNING->timp<=0){
					RUNNING=del(RUNNING);
					if(READY != NULL){
						if(proces_ales == 2){
						MIN=alegegere_SJF(READY);
						RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
						READY=del2(READY,MIN->nume);
						}
						else{
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							if(proces_ales == 3)
								timp_petrecut=0;
						}
					}
				}
				else{
					if(proces_ales == 3 && timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							timp_petrecut=0;
						}
				}
			}	
		}
		if(s[0]=='w'){
			WAITING=add2(WAITING,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
			RUNNING=del(RUNNING);
			if(READY != NULL){
				if(proces_ales== 2){
					MIN=alegegere_SJF(READY);
					RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
					READY=del2(READY,MIN->nume);
				}
				else{
					RUNNING=add2(RUNNING,READY->nume, READY->timp, READY->prioritate);
					READY=del(READY);
					if(proces_ales == 3)
						timp_petrecut=0;
				}
			}
		}
		if(s[0]=='e'){
			int i=2,k=0;
			char aux[21];
			while(i<strlen(s)){
				aux[k]=s[i];
				k++; i++;
			}
			aux[strlen(s)-3]='\0';
			proces *q=WAITING;
			while(q->next!=NULL && strcmp(q->nume,aux)!=0)
				q=q->next;
			READY=add2(READY,q->nume, q->timp, q->prioritate);
			if(RUNNING!=NULL){
				RUNNING=tick(RUNNING);
				timp_petrecut++;
				if(RUNNING->timp<=0){ //doamne ajuta
					RUNNING=del(RUNNING);
					if(READY != NULL){
						if(proces_ales== 2){
							MIN=alegegere_SJF(READY);
							RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
							READY=del2(READY,MIN->nume);
						}
						else{
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							if(proces_ales == 3)
								timp_petrecut=0;
						}
					}
				}
				else{
					if(proces_ales == 3 && timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							RUNNING=add2(RUNNING, READY->nume, READY->timp, READY->prioritate);
							READY=del(READY);
							timp_petrecut=0;
						}
				}
			}
			else{
				if(proces_ales== 2){
						MIN=alegegere_SJF(READY);
						RUNNING=add2(RUNNING, MIN->nume, MIN->timp, MIN->prioritate);
						READY=del2(READY,MIN->nume);
				}
				else{
					RUNNING=add2(RUNNING,READY->nume,READY->timp,READY->prioritate);
					READY=del(READY);
					if(proces_ales == 3)
						timp_petrecut=0;
				}
			}

			WAITING=del2(WAITING,aux);		
		}

		}
		else{
			if(proces_ales == 4){
				if(s[0]=='a'){
			READY=add(READY,s);
			if(RUNNING==NULL){
				if(READY!=NULL){
					MAX=alegere_PP(READY);
					RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
					READY=del2(READY, MAX->nume);	
					timp_petrecut=0;
				}
			}
			else{
				if(RUNNING!=NULL){
					RUNNING=tick(RUNNING);
					timp_petrecut++;
					if(RUNNING->timp==0){
						RUNNING=del(RUNNING);
						if(READY != NULL){
							MAX=alegere_PP(READY);
							RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
							READY=del2(READY, MAX->nume);
							timp_petrecut=0;
						}
					}
					else{
						if(timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							MAX=alegere_PP(READY);
							RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
							READY=del2(READY, MAX->nume);
							timp_petrecut=0;
						}
					}
					AUX_MAX=alegere_PP(READY);
					if(AUX_MAX->prioritate>RUNNING->prioritate){
						READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
						RUNNING=del(RUNNING);
						RUNNING=add2(RUNNING, AUX_MAX->nume, AUX_MAX->timp, AUX_MAX->prioritate);
						READY=del2(READY, AUX_MAX->nume);
						timp_petrecut=0;
					}
				}
			}
			
		}
		if(s[0]=='m'){
			READY=multiple_add(READY,s);
			if(RUNNING==NULL){
				if(READY!=NULL){
					MAX=alegere_PP(READY);
					RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
					READY=del2(READY, MAX->nume);	
					timp_petrecut=0;
				}
			}
			else{
				if(RUNNING!=NULL){
					RUNNING=tick(RUNNING);
					timp_petrecut++;
					if(RUNNING->timp==0){
						RUNNING=del(RUNNING);
						if(READY != NULL){
							MAX=alegere_PP(READY);
							RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
							READY=del2(READY, MAX->nume);
							timp_petrecut=0;
						}
					}
					else{
						if(timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							MAX=alegere_PP(READY);
							RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
							READY=del2(READY, MAX->nume);
							timp_petrecut=0;
						}
					}
					AUX_MAX=alegere_PP(READY);
					if(AUX_MAX->prioritate>RUNNING->prioritate){
						READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
						RUNNING=del(RUNNING);
						RUNNING=add2(RUNNING, AUX_MAX->nume, AUX_MAX->timp, AUX_MAX->prioritate);
						READY=del2(READY, AUX_MAX->nume);
						timp_petrecut=0;
					}
				}
			}
		}
		if(s[0]=='s'){
			if(RUNNING == NULL)
				fprintf(f2, "\n");
			else
			fprintf(f2, "%s %d\n", RUNNING->nume, RUNNING->timp);
		}
		if(s[0]=='t'){
			if(RUNNING!=NULL){
				RUNNING=tick(RUNNING);
				timp_petrecut++;
				if(RUNNING->timp<=0){
					RUNNING=del(RUNNING);
					if(READY != NULL){
						MAX=alegere_PP(READY);
						RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
						READY=del2(READY, MAX->nume);
						timp_petrecut=0;
					}
				}
				else{
					if(timp_petrecut == cuanta){
							READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
							RUNNING=del(RUNNING);
							MAX=alegere_PP(READY);
							RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
							READY=del2(READY, MAX->nume);
							timp_petrecut=0;
						}
				}
			}	
		}
		if(s[0]=='w'){
			if(RUNNING !=NULL){
				WAITING=add2(WAITING,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
				RUNNING=del(RUNNING);
			}
			if(READY != NULL){
				MAX=alegere_PP(READY);
				RUNNING=add2(RUNNING,MAX->nume, MAX->timp, MAX->prioritate);
				READY=del2(READY, MAX->nume);
				timp_petrecut=0;
			}
		}

		if(s[0]=='e'){
			int i=2,k=0;
			char aux[21];
			while(i<strlen(s)){
				aux[k]=s[i];
				k++; i++;
			}
			aux[strlen(s)-3]='\0';
			proces *q=WAITING;
			while(q->next!=NULL && strcmp(q->nume,aux)!=0)
				q=q->next;
			READY=add2(READY,q->nume, q->timp, q->prioritate);
			if(RUNNING!=NULL){
				RUNNING=tick(RUNNING);
				timp_petrecut++;
				if(RUNNING->timp<=0){ //doamne ajuta
					RUNNING=del(RUNNING);
					if(READY != NULL){
						MAX=alegere_PP(READY);
						RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
						READY=del2(READY, MAX->nume);
						timp_petrecut=0;
					}
				}
				else{
					if(timp_petrecut == cuanta){
						READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
						RUNNING=del(RUNNING);
						MAX=alegere_PP(READY);
						RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
						READY=del2(READY, MAX->nume);
						timp_petrecut=0;
					}
				}
				AUX_MAX=alegere_PP(READY);
					if(AUX_MAX->prioritate>RUNNING->prioritate){
						READY=add2(READY,RUNNING->nume, RUNNING->timp, RUNNING->prioritate);
						RUNNING=del(RUNNING);
						RUNNING=add2(RUNNING, AUX_MAX->nume, AUX_MAX->timp, AUX_MAX->prioritate);
						READY=del2(READY, AUX_MAX->nume);
						timp_petrecut=0;
					}
			}
			else{
				MAX=alegere_PP(READY);
				RUNNING=add2(RUNNING, MAX->nume, MAX->timp, MAX->prioritate);
				READY=del2(READY, MAX->nume);
				timp_petrecut=0;
			}

			WAITING=del2(WAITING,aux);
			
		}
			}
		}

	}
	fclose(f2);
	fclose(f);
	return 0;
}