#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define ERR -1
#define HASH_SIZE 100

typedef struct DosarCandidat
{
	char* numeCandidat;
	char* programStudiu;
	float medieBac;
	int codDosar;
}DosarCandidat;

typedef struct Nod
{
	DosarCandidat* d;
	struct Nod* next;
}Nod;

typedef struct NodL
{
	char* programStudiu;
	Nod* root;
	struct NodL* next;
}NodL;

unsigned char hash_function(const char* key)
{
	if (!key)
		return ERR;

	return key[0] % HASH_SIZE;
}

void inserare(Nod** ht, DosarCandidat* d)
{
	unsigned char key = hash_function(d->numeCandidat);

	if (ht[key] == NULL)
	{
		ht[key] = (Nod*)malloc(sizeof(Nod));
		ht[key]->d = (DosarCandidat*)malloc(sizeof(DosarCandidat));
		ht[key]->d->numeCandidat = (char*)malloc((strlen(d->numeCandidat) + 1) * sizeof(char));
		ht[key]->d->programStudiu = (char*)malloc((strlen(d->programStudiu) + 1) * sizeof(char));
		strcpy(ht[key]->d->numeCandidat, d->numeCandidat);
		strcpy(ht[key]->d->programStudiu, d->programStudiu);
		ht[key]->d->medieBac = d->medieBac;
		ht[key]->d->codDosar = d->codDosar;

		ht[key]->next = NULL;
	}
	else
	{
		Nod* it = ht[key];
		while (it->next != NULL)
		{
			it = it->next;
		}

		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod = (Nod*)malloc(sizeof(Nod));
		nod->d = (DosarCandidat*)malloc(sizeof(DosarCandidat));
		nod->d->numeCandidat = (char*)malloc((strlen(d->numeCandidat) + 1) * sizeof(char));
		nod->d->programStudiu = (char*)malloc((strlen(d->programStudiu) + 1) * sizeof(char));
		strcpy(nod->d->numeCandidat, d->numeCandidat);
		strcpy(nod->d->programStudiu, d->programStudiu);
		nod->d->medieBac = d->medieBac;
		nod->d->codDosar = d->codDosar;

		nod->next = NULL;

		it->next = nod;
	}
}

unsigned char getNrStudenti(Nod** ht, const char* programStudiu)
{
	unsigned char count = 0;
	for (unsigned char i = 0; i < HASH_SIZE; i++)
	{
		Nod* it = ht[i];
		while (it != NULL)
		{
			if (!strcmp(it->d->programStudiu, programStudiu))
			{
				count++;
			}

			it = it->next;
		}
	}

	return count;
}

void adaugaInLDL(NodL* ldl, Nod* root)
{
	NodL* itL = ldl;

	if (itL->programStudiu == NULL)
	{
		itL->programStudiu = (char*)malloc((strlen(root->d->programStudiu) + 1) * sizeof(char));
		strcpy(itL->programStudiu, root->d->programStudiu);
	}
	else
	{
		while (itL != NULL)
		{
			if (!strcmp(itL->programStudiu, root->d->programStudiu)) 
			{
				break;
			}
			else
			{
				if (itL->next == NULL)
				{
					itL->next = (Nod*)malloc(sizeof(Nod));
					itL = itL->next;
					itL->root = NULL;
					itL->next = NULL;
					itL->programStudiu = (char*)malloc((strlen(root->d->programStudiu) + 1) * sizeof(char));
					strcpy(itL->programStudiu, root->d->programStudiu);
				}
				else
				{
					itL = itL->next;
				}
			}
		}
	}



	if (itL->root == NULL)
	{
		itL->root = (Nod*)malloc(sizeof(Nod));
		itL->root->d = (DosarCandidat*)malloc(sizeof(DosarCandidat));
		itL->root->d->numeCandidat = (char*)malloc((strlen(root->d->numeCandidat) + 1) * sizeof(char));
		itL->root->d->programStudiu = (char*)malloc((strlen(root->d->programStudiu) + 1) * sizeof(char));
		strcpy(itL->root->d->numeCandidat, root->d->numeCandidat);
		strcpy(itL->root->d->programStudiu, root->d->programStudiu);
		itL->root->d->medieBac = root->d->medieBac;
		itL->root->d->codDosar = root->d->codDosar;
		itL->root->next = NULL;
	}
	else
	{
		Nod* it = itL->root;
		while (it->next != NULL)
		{
			it = it->next;
		}

		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->d = (DosarCandidat*)malloc(sizeof(DosarCandidat));
		nod->d->numeCandidat = (char*)malloc((strlen(root->d->numeCandidat) + 1) * sizeof(char));
		nod->d->programStudiu = (char*)malloc((strlen(root->d->programStudiu) + 1) * sizeof(char));
		strcpy(nod->d->numeCandidat, root->d->numeCandidat);
		strcpy(nod->d->programStudiu, root->d->programStudiu);
		nod->d->medieBac = root->d->medieBac;
		nod->d->codDosar = root->d->codDosar;
		nod->next = NULL;

		it->next = nod;
	}
}

void populareLDL(NodL* ldl, Nod** ht, float medieMaxima)
{
	for (unsigned char i = 0; i < HASH_SIZE; i++)
	{
		Nod* it = ht[i];
		while (it != NULL)
		{
			if (it->d->medieBac <= medieMaxima)
			{
				adaugaInLDL(ldl, it);
			}
			it = it->next;
		}
	}
}

float getMinMedie(NodL* ldl)
{
	NodL* itL = ldl;
	float min = itL->root->d->medieBac;
	while (itL != NULL)
	{
		Nod* it2 = itL->root;
		while (it2 != NULL)
		{
			if (min > it2->d->medieBac)
			{
				min = it2->d->medieBac;
			}
			it2 = it2->next;
		}
		itL = itL->next;
	}

	return min;
}

void stergeDosarLDL(NodL* ldl)
{
	float min = getMinMedie(ldl);
	NodL* itL = ldl;
	while (itL != NULL)
	{
		Nod* it2 = itL->root;
		while (it2->next != NULL)
		{
			if (it2->next->d->medieBac == min)
			{
				Nod* aux = it2->next;
				if(it2->next->next != NULL)
					it2->next = it2->next->next;
				else
				{
					it2->next = NULL;
				}
				free(aux->d->numeCandidat);
				free(aux->d->programStudiu);
				free(aux->d);
				free(aux->next);
				free(aux);

				break;
			}
			it2 = it2->next;
		}

		itL = itL->next;
	}
}

void main()
{
	Nod** ht = (Nod**)malloc(HASH_SIZE * sizeof(Nod*));
	for (unsigned char i = 0; i < HASH_SIZE; i++)
	{
		ht[i] = NULL;
	}

	FILE* f;
	f = fopen("date.txt", "r+");

	char numeCandidat[64];
	char programStudiu[64];

	while (!feof(f))
	{
		DosarCandidat d;

		fscanf(f, "%[^,],", &numeCandidat);
		fscanf(f, "%[^,],", &programStudiu);
		fscanf(f, "%f,", &d.medieBac);
		fscanf(f, "%d\n", &d.codDosar);

		d.numeCandidat = (char*)malloc((strlen(numeCandidat) + 1) * sizeof(char));
		d.programStudiu = (char*)malloc((strlen(programStudiu) + 1) * sizeof(char));

		strcpy(d.numeCandidat, numeCandidat);
		strcpy(d.programStudiu, programStudiu);

		inserare(ht, &d);
	}


	for (unsigned char i = 0; i < HASH_SIZE; i++)
	{
		Nod* it = ht[i];
		while (it != NULL)
		{
			printf("%d    %s, %s, %.2f, %d\n",
				i,
				it->d->numeCandidat,
				it->d->programStudiu,
				it->d->medieBac,
				it->d->codDosar);

			it = it->next;
		}
	}

	printf("\n\nNr. studenti la ID: %d\n", getNrStudenti(ht, "ID"));
	printf("Nr. studenti la IF: %d\n\n", getNrStudenti(ht, "IF"));

	NodL listaDeListe;
	listaDeListe.next = NULL;
	listaDeListe.programStudiu = NULL;
	listaDeListe.root = NULL;
	populareLDL(&listaDeListe, ht, 10.0);

	NodL* iTL = &listaDeListe;
	while (iTL != NULL)
	{
		printf("INVATAMANT %s:\n", iTL->programStudiu);
		Nod* it2 = iTL->root;
		while (it2 != NULL)
		{
			printf("%s,%s,%.2f,%d\n",
				it2->d->numeCandidat,
				it2->d->programStudiu,
				it2->d->medieBac,
				it2->d->codDosar);

			it2 = it2->next;
		}
		printf("\n");
		iTL = iTL->next;
	}


	stergeDosarLDL(&listaDeListe);
	{
		NodL* iTL = &listaDeListe;
		while (iTL != NULL)
		{
			printf("INVATAMANT %s:\n", iTL->programStudiu);
			Nod* it2 = iTL->root;
			while (it2 != NULL)
			{
				printf("%s,%s,%.2f,%d\n",
					it2->d->numeCandidat,
					it2->d->programStudiu,
					it2->d->medieBac,
					it2->d->codDosar);

				it2 = it2->next;
			}
			printf("\n");
			iTL = iTL->next;
		}
	}
}