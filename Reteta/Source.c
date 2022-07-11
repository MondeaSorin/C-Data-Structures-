#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// liste duble, hash-maps(vector de liste), lista de liste, AVL, arbori binari, heap
#define VECTOR_SIZE 1000


typedef struct Reteta
{
	unsigned int crtReteta;
	unsigned char nrMedicamente;
	char* numePacient;
	char* numeMedic;
	float valoareReteta;
}Reteta;

typedef struct Nod
{
	Reteta* info;
	struct Nod* stanga;
	struct Nod* dreapta;
}Nod;

Nod* createNod(Reteta* reteta)
{
	Nod* root = (Nod*)malloc(sizeof(Nod));

	if (reteta == NULL) // 0xcc cc cc cc ?
	{
		return NULL;
	}
	root->info = (Reteta*)malloc(sizeof(Reteta));
	Reteta* nou = root->info;
	nou->crtReteta = reteta->crtReteta;
	nou->nrMedicamente = reteta->nrMedicamente;

	nou->numePacient = (char*)malloc((strlen(reteta->numePacient) + 1) * sizeof(char));
	strcpy(nou->numePacient, reteta->numePacient);

	nou->numeMedic = (char*)malloc((strlen(reteta->numeMedic) + 1) * sizeof(char));
	strcpy(nou->numeMedic, reteta->numeMedic);

	nou->valoareReteta = reteta->valoareReteta;

	root->stanga = NULL;
	root->dreapta = NULL;

	return root;
}

void adaugare(Nod* root, Nod* nou)
{
	if (nou == NULL)
	{
		return;
	}

	Nod* ref = root;

	while (1)
	{
		if (nou->info->crtReteta < ref->info->crtReteta)
		{
			if (ref->stanga == NULL)
			{
				ref->stanga = nou;
				return;
			}
			else
			{
				ref = ref->stanga;
			}
		}
		else
		{
			if (ref->dreapta == NULL)
			{
				ref->dreapta = nou;
				return;
			}
			else
			{
				ref = root->dreapta;
			}
		}
	}
}

void afisare(Nod* root)
{
	if (root == NULL)
	{
		return;
	}

	afisare(root->dreapta);

	// Write your code here...
	printf("[%d] -> [%s]\n", root->info->crtReteta, root->info->numePacient);

	afisare(root->stanga);
	//
}

unsigned char getNrRetete(Nod* root ,const char* numePacient)
{
	if (root == NULL)
	{
		return 0;
	}
	unsigned char found = 0;
	if (!strcmp(root->info->numePacient, numePacient))
	{
		found = 1;
	}
	return getNrRetete(root->stanga, numePacient) + getNrRetete(root->dreapta, numePacient) + found;
}

unsigned char maxim(unsigned char stanga, unsigned char dreapta)
{
	return stanga >= dreapta ? stanga : dreapta;
}

unsigned char getAdancime(Nod* root)
{
	if (root == NULL)
	{
		return 0;
	}
	return maxim(getAdancime(root->stanga), getAdancime(root->dreapta)) + 1;
}

unsigned char getSuntEchilibrat(Nod* root)
{
	// daca stanga echilibrat si dreapta echilibrat atunci sunt echilibrat ?????????????
	char diferenta = getAdancime(root->stanga) - getAdancime(root->dreapta);
	
	// daca dif > 1 atunci echilibreaza(root) -> rotatie la dreapta 
	// daca dif < -1 atunci echilibraza -> rotatie la stanga
	
	if (diferenta < 0)
	{
		diferenta = -diferenta;
	}


	return diferenta <= 1 ? 1 : 0;
}

unsigned char getStare(Nod* root)
{
	// outputul operator&& copmpatibil cu unsigned char?

	if (root == NULL)
	{
		return 1;
	}

	return getStare(root->stanga) && getStare(root->dreapta) && getSuntEchilibrat(root) == 1 ? 1 : 0; // 1 / 0 -> 1->echilibrat , 0->ne-echilibrat
}

void getRetete(Nod* root, const char* numeMedic, unsigned char* nrRetete, Reteta* listaRetete)
{
	if (root == NULL)
	{
		return;
	}

	if (root->info->nrMedicamente >= 3 && !strcmp(root->info->numeMedic, numeMedic))
	{
		listaRetete[(*nrRetete)++] = *root->info;
	}

	getRetete(root->stanga, numeMedic, nrRetete, listaRetete);
	getRetete(root->dreapta, numeMedic, nrRetete, listaRetete);
}

void afiseazaReteta(Reteta r)
{
	printf("Reteta{idReteta=%d, nrMedicamente=%d, numePacient='%s', numeDoctor='%s', valoare=%2.f}\n", 
		r.crtReteta,
		r.nrMedicamente,
		r.numePacient,
		r.numeMedic,
		r.valoareReteta);
}

void main()
{
	FILE* f;
	f = fopen("Retete.txt", "r+");
	

	Reteta* r = (Reteta*)malloc(sizeof(Reteta));
	r->crtReteta = 5;
	r->nrMedicamente = 5;
	r->numePacient = (char*)malloc((strlen("Alex") + 1) * sizeof(char));
	r->numeMedic = (char*)malloc((strlen("George") + 1) * sizeof(char));

	strcpy(r->numePacient, "Alex");
	strcpy(r->numeMedic, "George");

	r->valoareReteta = 100.5;

	Reteta* r2 = (Reteta*)malloc(sizeof(Reteta));
	r2->crtReteta = 3;
	r2->nrMedicamente = 5;
	r2->numePacient = (char*)malloc((strlen("Alex") + 1) * sizeof(char));
	r2->numeMedic = (char*)malloc((strlen("George") + 1) * sizeof(char));

	strcpy(r2->numePacient, "Alex");
	strcpy(r2->numeMedic, "George");

	r2->valoareReteta = 100.5;

	Nod* root = createNod(r);
	adaugare(root, createNod(r2));

	afisare(root);
	printf("Nr retetelor care apartin lui Alex sunt: %d\n", getNrRetete(root, "Alex"));
	char stare[100];
	strcpy(stare, getStare(root) == 1 ? "Echilibrat" : "Neecilibrat");
	printf("Starea arborelui este : %s\n", stare);
	Reteta* retete = (Reteta*)malloc(VECTOR_SIZE * sizeof(Reteta));
	unsigned char nrElemente = 0;

	getRetete(root, "George", &nrElemente, retete);
	
	printf("================= LISTA RETETE =================\n");
	for (unsigned char i = 0; i < nrElemente; i++)
	{
		afiseazaReteta(retete[i]);
	}
	printf("================================================\n");

	free(retete);
	

}