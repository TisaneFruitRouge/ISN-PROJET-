#define OLC_PGE_APPLICATION


#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
//using namespace std;

#include "olcPixelGameEngine.h"


//On initialise différentes variables

int largeurEcran = 1800;
int hauteurEcran = 1000;

bool couleurJaune = true;
int couleurA = 200;
int couleurB = 200;
int couleurC = 0;
int vie = 4;
int victoire = 0;
int bullet = 6;
int tempsUnix = time(NULL);
int tempsBullet = time(NULL);
int tempsMonstre = time(NULL);
int tempsRechargement = 9000000000000;
bool teleporteurOn = false;

clock_t repoptime = clock();
clock_t temps = clock();

class Projet : public olc::PixelGameEngine
{
public:
	Projet()
	{
		sAppName = "Marcel contre Satan chez Sa tante";
	}

private:
	std::string MAP;
	std::string MAP1;
	std::string MAPBOSS;
	int largeurNiveau;
	int hauteurNiveau;
	int largeurPixel;

	bool premierTour = true;
	bool MAP1_finie = false;
	//On définit des structures pour les différents types d'objets, ainsi si on parle d'une balle par exemple on a dans un seul élément tout ce qui la définit (sa vitesse, sa position...)

	struct Monstre
	{

		float x;
		float y;
		double velX;
		double velY;
		int Vie = 300;

		bool enCollisionX = false;
		bool enCollisionY = false;

		olc::Sprite *sprite = nullptr;

	};

	struct Joueur {		//Création de la structure du joueur

		float xPosJoueur;
		float yPosJoueur;
		float velXJoueur;
		float velYJoueur;
		float velBase = 200.0;
		int demiCoeur = vie;


		olc::Sprite * spriteJoueur;
		olc::Sprite * DemiCoeurGauche;
		olc::Sprite * DemiCoeurDroit;
		olc::Sprite * messagedemort;
		olc::Sprite * messagedevictoire;
		olc::Sprite * bullet;

	};

	struct Bullet {		//Création de la structure de la balle

		float x;
		float y;
		float vx;
		float vy;
		bool Suppr;
		float Vitesse = 15.0;

		olc::Sprite* sprite;

	};

	struct PixelJaune {		//Structure d'un type de bloc
		float x;
		float y;
		olc::Pixel couleur;
	};

	struct Bloc {		//Création de la structure du bloc

		float x;
		float y;
		olc::Pixel couleur;

	};

	struct Boss {

		float x;
		float y;
		int Vie_MAX = 3000;
		int Vie = Vie_MAX;

	};

	//On crée des listes pour chaque objets pour regrouper tous les objets du même type en un même "emplacement"

	std::vector<Bullet> listBullets;
	std::vector<PixelJaune> listPixelJaune;
	std::vector<Bloc> listBlocs;
	std::vector<Monstre> listMonstre;
	std::vector<Bullet> listBulletBoss;
	Joueur j;
	Boss LeBoss;
	Bloc teleporteur;
	

	//On initialise différentes variables pour plus tard, ce sont celle des colisions ici on ne fait que les créer et plus tard on explique quand elles sont égals à true ou false

	bool MonstreColiJoueur(Joueur J, Monstre M);
	bool BlocColiBullet(Bloc Bl, Bullet Bu);
	bool PixelJauneColiBullet(PixelJaune Bl, Bullet Bu);
	std::vector<bool> BlocColiJoueur(Bloc Bl, Joueur j);
	bool BlocColiJoueurBool(Bloc Bl, Joueur J);
	bool BulletColiMonstre(Bullet Bu, Monstre M);
	bool BulletColiJoueur(Bullet Bu, Joueur J);
	std::vector<double>GetCoordsVelMonstre(Monstre M, Joueur J);
	std::vector<bool>BlocColiMonstre(Bloc Bl, Monstre M);
	std::vector<bool>MonstreColiMonstre(Monstre M1, std::vector<Monstre> ListMontre);
	Bullet BossShootsBullet(Boss B, Joueur J);
	Monstre BossSpawnMonstre(Boss B, Joueur J);
	void afficherVieBoss(Boss B);
	bool BulletColiBoss(Bullet Bu, Boss Bo);


protected:
	virtual bool OnUserCreate() override
	{
		largeurNiveau = 90;				//On définit la taille du niveau et la taille de chaque parcelle
		hauteurNiveau = 50;
		largeurPixel = 20;

		j.DemiCoeurGauche = new olc::Sprite("Sprites/CoeurGauche.png"); //Sprite du coeur
		j.DemiCoeurDroit = new olc::Sprite("Sprites/CoeurDroit.png");
		j.messagedemort = new olc::Sprite("Sprites/messagedemort.png");
		j.messagedevictoire = new olc::Sprite("Sprites/messagedevictoire.png");
		j.bullet = new olc::Sprite("Sprites/Bullet.png");

		//On dessine la map
		MAP1 = MAP1 + ".........................................................................................."; // "." correspond à un espace vide
		MAP1 = MAP1 + "............................................................M............................."; // "M" correspond à un monstre
		MAP1 = MAP1 + ".........................................................................................."; // "%"correspond à un bloc
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "...........................................................................M..............";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + ".......................................................M..................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "....................................%.....................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "............................................................%.............................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + ".................%........................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................%........................M......................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "............................................................................M.............";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";
		MAP1 = MAP1 + "..........................................................................................";


		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + ".........................................%%%..............................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "............%..............................................................%..............";
		MAPBOSS = MAPBOSS + "............%.............................B................................%..............";
		MAPBOSS = MAPBOSS + "............%..............................................................%..............";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + ".........................................%%%..............................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		MAPBOSS = MAPBOSS + "..........................................................................................";
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		int LCarreau = 32;
		int HCarreau = 32;

		if (MAP1_finie == false) MAP = MAP1;
		else MAP = MAPBOSS;

		//Quand on appuye sur une touche
		if (GetKey(olc::Key::Z).bPressed) {
			j.velYJoueur = -j.velBase * fElapsedTime;
		}
		if (GetKey(olc::Key::S).bPressed) {
			j.velYJoueur = j.velBase * fElapsedTime;
		}
		if (GetKey(olc::Key::D).bPressed) {
			j.velXJoueur = j.velBase * fElapsedTime;
		}
		if (GetKey(olc::Key::Q).bPressed) {
			j.velXJoueur = -j.velBase * fElapsedTime;
		}

		//Quand on lache la touche
		if (GetKey(olc::Key::Z).bReleased) {
			j.velYJoueur = 0;
		}
		else if (GetKey(olc::Key::S).bReleased) {
			j.velYJoueur = 0;
		}
		if (GetKey(olc::Key::D).bReleased) {
			j.velXJoueur = 0;
		}
		else if (GetKey(olc::Key::Q).bReleased) {
			j.velXJoueur = 0;
		}

		if (GetKey(olc::Key::SPACE).bPressed && bullet > 0) {


			//Définition de la structure de la balle c'est à dire à quoi elle ressemble, qu'elle est sa position de départ et sa vitesse 

			Bullet o;
			o.x = j.xPosJoueur + largeurPixel;
			o.y = j.yPosJoueur + largeurPixel;
			o.sprite = new olc::Sprite("Sprites/Bullet.png");


			//On utilise les formules trigonométriques pour calculer la vitesse, on donne les coordonnées de l'intesection entre la demi droite partant du personnage allant vers la souris et le cercle trigonométrique (ayant pour centre le personnage)

			if ((o.x - GetMouseX()) < 0)
			{
				o.vx = cos(atan((GetMouseY() - o.y) / (GetMouseX() - o.x))) * o.Vitesse; //* fElapsedTime;
				o.vy = sin(atan((GetMouseY() - o.y) / (GetMouseX() - o.x))) * o.Vitesse; //* fElapsedTime;
			}
			else if ((o.x - GetMouseX()) > 0)
			{
				o.vx = -cos(atan((GetMouseY() - o.y) / (GetMouseX() - o.x))) * o.Vitesse; //* fElapsedTime;
				o.vy = -sin(atan((GetMouseY() - o.y) / (GetMouseX() - o.x))) * o.Vitesse; //* fElapsedTime;
			}
			else if ((o.x - GetMouseX()) == 0)
			{
				o.vx = 0;
				o.vy = o.Vitesse; //*fElapsedTime;
			}
			listBullets.push_back(o);  //on rajoute la balle à la liste
			bullet -= 1;
		}

		if (bullet == 0) {
			tempsRechargement = time(NULL);
			bullet = -6;
		}
		if (tempsRechargement <= time(NULL) - 3 && bullet == -6) {
			bullet = 6;
			tempsRechargement = 9000000000000;
		}


		//Création de la MAP
		int i;
		int l;

		//On crée chaque pixel (par pixel on entends un block composé de 20 * 20 pixel de l'écran) un à un
		//On sépare selon chaque chose dans la map pour que chaque figure telle qu'un point correspondent à un élément

		for (i = 0; i < largeurNiveau; i++) {
			for (l = 0; l < hauteurNiveau; l++) {



				if (LeBoss.Vie <= 0) {
					int s = 0;
					int k = 0;
					int ks = 0;
					int kss = 0;
					for (kss = 0; kss < listBullets.size(); kss++) {
						listBullets.erase(listBullets.begin());
					}
					for (ks = 0; ks < listBulletBoss.size(); s++) {
						listBulletBoss.erase(listBulletBoss.begin());
					}
					for (s = 0; s < listBlocs.size(); s++) {
						listBlocs.erase(listBlocs.begin());
					}
					for (k = 0; k < listMonstre.size(); k++) {
						listMonstre.erase(listMonstre.begin());
					}

					FillRect(i * largeurPixel, l * largeurPixel, largeurPixel, largeurPixel, olc::Pixel(0, 0, 0));
					j.xPosJoueur = -1000000;	//L'on téléporte le personnage très loin et l'on réduit sa vitesse à 0 car c'est plus simple que de le faire disparaître
					j.yPosJoueur = -1000000;
					j.velXJoueur = 0;
					j.velYJoueur = 0;
					LeBoss.x = 100000;
					LeBoss.y = 100000;
					victoire = 1;
				}
				else if (j.demiCoeur > 0) {
					if (MAP[largeurNiveau * l + i] == '.') {
						FillRect(i * largeurPixel, l * largeurPixel, largeurPixel, largeurPixel, olc::Pixel(0, 0, 255));
					}
					else if (MAP[largeurNiveau * l + i] == '#') {

						PixelJaune b;
						b.x = i * largeurPixel;
						b.y = l * largeurPixel;
						b.couleur = olc::Pixel(200, 200, 0);
						listPixelJaune.push_back(b);

					}
					else if (MAP[largeurNiveau * l + i] == '%' and premierTour == true) {
						Bloc b;
						b.x = i * largeurPixel;
						b.y = l * largeurPixel;
						b.couleur = olc::Pixel(102, 0, 255);
						listBlocs.push_back(b);
					}
					else if (MAP[largeurNiveau * l + i] == 'M' and premierTour == true) {
						Monstre m;
						m.x = i * largeurPixel;
						m.y = l * largeurPixel;
						listMonstre.push_back(m);
						FillRect(i * largeurPixel, l * largeurPixel, largeurPixel, largeurPixel, olc::Pixel(0, 0, 255));
					}
					else if (MAP[largeurNiveau * l + i] == 'B' and premierTour == true)
					{
						LeBoss.x = i * largeurPixel;
						LeBoss.y = l * largeurPixel;
					}
					else FillRect(i * largeurPixel, l * largeurPixel, largeurPixel, largeurPixel, olc::Pixel(0, 0, 255));

				}
				else if (j.demiCoeur <= 0) {							//Quand la vie du personnage atteint 0 l'on supprime tout les éléments pour créer un écran noir
					int s = 0;
					int k = 0;
					int ks = 0;
					int kss = 0;
					for (kss = 0; kss < listBullets.size(); kss++) {
						listBullets.erase(listBullets.begin());
					}
					for (ks = 0; ks < listBulletBoss.size(); s++) {
						listBulletBoss.erase(listBulletBoss.begin());
					}
					for (s = 0; s < listBlocs.size(); s++) {
						listBlocs.erase(listBlocs.begin());
					}
					for (k = 0; k < listMonstre.size(); k++) {
						listMonstre.erase(listMonstre.begin());
					}

					FillRect(i * largeurPixel, l * largeurPixel, largeurPixel, largeurPixel, olc::Pixel(0, 0, 0));
					j.xPosJoueur = -1000000;	//L'on téléporte le personnage très loin et l'on réduit sa vitesse à 0 car c'est plus simple que de le faire disparaître
					j.yPosJoueur = -1000000;
					j.velXJoueur = 0;
					j.velYJoueur = 0;
					LeBoss.x = 100000;
					LeBoss.y = 100000;
					victoire = 2;
				}
			}

		}

		if (victoire == 2) { DrawSprite(700, 200, j.messagedemort, 1); }
		if (victoire == 1) { DrawSprite(700, 200, j.messagedevictoire, 1); }

		premierTour = false;  //On veut pas que les blocks se recrée en boucle sinon cela va surcharger la mémoire et causer des lags

		//Déplacement des balles

		for (l = 0; l < listBullets.size(); l++) {


			listBullets[l].x += listBullets[l].vx; //on ajoute la vélocité de la balle à sa position
			listBullets[l].y += listBullets[l].vy;


			DrawSprite(listBullets[l].x, listBullets[l].y, listBullets[l].sprite, 1); //On dessine la balle

		}

		//On va vérifier la colision en boucle entre une balle et le pixel jaune et si il y en a une les couleurs du pixel sont changés

		int n = 0;

		for (n = 0; n < listPixelJaune.size(); n++) {

			//On redessine à chaque passage le pixel mais ces couleurs dépendent de si la balle est déjà passé

			FillRect(listPixelJaune[n].x, listPixelJaune[n].y, largeurPixel, largeurPixel, olc::Pixel(couleurA, couleurB, couleurC));

			int bulletCount;

			//À chaque passage de la balle sur le bloc la variable couleurJaune va changer de true à false ou de false à true, ainsi si elle est true le prochain passage changera les couleurs vers du rouge et si c'est false le prochain fera du jaune

			for (bulletCount = 0; bulletCount < listBullets.size(); bulletCount++)
			{
				if (couleurJaune == true && PixelJauneColiBullet(listPixelJaune[n], listBullets[bulletCount]) == true)
				{
					couleurA = 255;
					couleurB = 0;
					couleurC = 0;
					FillRect(listPixelJaune[n].x, listPixelJaune[n].y, largeurPixel, largeurPixel, olc::Pixel(couleurA, couleurB, couleurC));
					couleurJaune = false;
				}
				else if (couleurJaune == false && PixelJauneColiBullet(listPixelJaune[n], listBullets[bulletCount]) == true)
				{
					couleurA = 200;
					couleurB = 200;
					couleurC = 0;
					FillRect(listPixelJaune[n].x, listPixelJaune[n].y, largeurPixel, largeurPixel, olc::Pixel(couleurA, couleurB, couleurC));
					couleurJaune = true;
				}
			}
		}


		if (MAP1_finie == true && tempsMonstre + 3 < time(NULL))
		{
			listMonstre.push_back(BossSpawnMonstre(LeBoss, j));
			tempsMonstre = time(NULL);
		}

		int m = 0;

		bool ColEnX = false;
		bool ColEnY = false;

		//On vérifie si on doit détruire chaque balles, ainsi si ellesont entrer en colision avec un bloc on les supprime (=erase()), idem si elles sortent du terrain

		int patate = 0;
		for (patate = 0; patate < listMonstre.size(); patate++)
		{
			listMonstre[patate].velX = GetCoordsVelMonstre(listMonstre[patate], j)[0]; //Calcule velocité du monstre en x
			listMonstre[patate].velY = GetCoordsVelMonstre(listMonstre[patate], j)[1]; //Calcule velocité du monstre en y
		}

		i = 1;
		int po = 1;

		if (LeBoss.Vie > 0) {

			for (i = 1; i <= j.demiCoeur; i++)//Dessin des coeurs
			{

				if (i % 2 == 1) {
					DrawSprite(1600 + 20 * (i - 1), 20, j.DemiCoeurGauche, 1);
				}
				else if (i % 2 == 0) {
					DrawSprite(1600 + 20 * (i - 1), 20, j.DemiCoeurDroit, 1);
				}

			}
			for (po = 1; po <= bullet; po++)//Dessin des balles
			{

				if (po % 2 == 1) {
					DrawSprite(1600 + 20 * (po - 1), 5, j.bullet, 1);
				}
				else if (po % 2 == 0) {
					DrawSprite(1600 + 20 * (po - 1), 5, j.bullet, 1);
				}

			}
		}

		for (m = 0; m < listBlocs.size(); m++) {

			FillRect(listBlocs[m].x, listBlocs[m].y, largeurPixel, largeurPixel, listBlocs[m].couleur); //Dessin des blocs

			int bulletCount;

			for (bulletCount = 0; bulletCount < listBullets.size(); bulletCount++)
			{
				if (BlocColiBullet(listBlocs[m], listBullets[bulletCount]) == true || listBullets[bulletCount].x > largeurEcran || listBullets[bulletCount].y > hauteurEcran || listBullets[bulletCount].x < 0 || listBullets[bulletCount].y < 0)
				{
					listBullets.erase(listBullets.begin() + bulletCount);
				}
				int m = 0;
				for (m = 0; m < listMonstre.size(); m++)
				{
					if (BulletColiMonstre(listBullets[bulletCount], listMonstre[m]))
					{
						listBullets.erase(listBullets.begin() + bulletCount);
						listMonstre[m].Vie -= 100;
					}
				}
				if (BulletColiBoss(listBullets[bulletCount], LeBoss))
				{
					listBullets.erase(listBullets.begin() + bulletCount);
					LeBoss.Vie -= 150;
				}
			}

			int BB = 0;

			for (BB = 0; BB < listBulletBoss.size(); BB++)
			{
				if (BlocColiBullet(listBlocs[m], listBulletBoss[BB]))
				{
					listBulletBoss.erase(listBulletBoss.begin() + BB);
				}
			}

			std::vector<bool> coords;

			//On vérifie les collisions de blocs avec le joueur, si c'est le cas sa vitesse est réduite à 0 dans cette direction 

			coords = BlocColiJoueur(listBlocs[m], j);
			if (ColEnX == false) ColEnX = coords[0];
			if (ColEnY == false) ColEnY = coords[1];

			//Vérif collision entre un monstre et un bloc

			int MonstreCount;
			for (MonstreCount = 0; MonstreCount < listMonstre.size(); MonstreCount++)
			{
				if (listMonstre[MonstreCount].enCollisionX == false) { listMonstre[MonstreCount].enCollisionX = BlocColiMonstre(listBlocs[m], listMonstre[MonstreCount])[0]; }//Si le monstre n'est pas déja un colision, on vérifie qu'il l'est.
				if (listMonstre[MonstreCount].enCollisionY == false) { listMonstre[MonstreCount].enCollisionY = BlocColiMonstre(listBlocs[m], listMonstre[MonstreCount])[1]; }
			}
		}

		//Check Bullet Coli Boss
		if (MAP1_finie) {

			int bulletCount;

			for (bulletCount = 0; bulletCount < listBullets.size(); bulletCount++)
			{
				if (BulletColiBoss(listBullets[bulletCount], LeBoss)) {
					LeBoss.Vie -= 100;
					listBullets.erase(listBullets.begin() + bulletCount);
				}
			}


			Bullet o;
			if (tempsBullet + 1 < time(NULL)) {
				listBulletBoss.push_back(BossShootsBullet(LeBoss, j));
				tempsBullet = time(NULL);
			}

			int BB;

			for (BB = 0; BB < listBulletBoss.size(); BB++)
			{

				if (BulletColiJoueur(listBulletBoss[BB], j)) {
					listBulletBoss.erase(listBulletBoss.begin() + BB);
					j.demiCoeur -= 1;
				}

				listBulletBoss[BB].x += listBulletBoss[BB].vx; //on ajoute la vélocité de la balle à sa position
				listBulletBoss[BB].y += listBulletBoss[BB].vy;
				DrawSprite(listBulletBoss[BB].x, listBulletBoss[BB].y, listBulletBoss[BB].sprite, 1);
			}
		}


		int MonstreCount2;
		for (MonstreCount2 = 0; MonstreCount2 < listMonstre.size(); MonstreCount2++)
		{

			if (MonstreColiJoueur(j, listMonstre[MonstreCount2]))
			{
				j.demiCoeur -= 1;
				repoptime = clock();
			}
		}

		if (MAP1_finie == false)
		{
			if (listMonstre.size() == 0 && teleporteurOn == false){
			
				teleporteur.x = largeurEcran / 2 - largeurPixel;
				teleporteur.y = hauteurEcran / 2 - largeurPixel;
				teleporteurOn = true;
			}

			if (teleporteurOn){
				FillRect(teleporteur.x, teleporteur.y, 2 * largeurPixel, 2 * largeurPixel, olc::Pixel(25, 200, 25));
				if (BlocColiJoueurBool(teleporteur, j))
				{
					j.xPosJoueur = 100;
					j.yPosJoueur = hauteurEcran / 2;
					MAP1_finie = true;
					premierTour = true;

					int c = 0;
						for (c = 0; c <= listBlocs.size() + 2; c++)
						{
							listBlocs.erase(listBlocs.begin());
						}
				}
			}

		}

		// On fixe la vélocité du joueur à 0 si il rencontre un bloc ou s'il s'apprete à sortir de l'écran
		if (ColEnX == true || j.xPosJoueur + j.velXJoueur + 2 * largeurPixel > largeurEcran || j.xPosJoueur + j.velXJoueur < 0) { j.velXJoueur = 0; }
		if (ColEnY == true || j.yPosJoueur + j.velYJoueur + 2 * largeurPixel > hauteurEcran || j.yPosJoueur + j.velYJoueur < 0) { j.velYJoueur = 0; }


		if (temps < clock() - 10) {			//On fait en sorte que les déplacements du joueurs sont dépendants du temps et non pas du nombre de boucle par secondes
			j.xPosJoueur += j.velXJoueur;	//La position change en fonction de la vélocité
			j.yPosJoueur += j.velYJoueur;
			temps = clock();
		}

		FillRect(j.xPosJoueur, j.yPosJoueur, 2 * largeurPixel, 2 * largeurPixel, olc::DARK_GREEN); // on dessine le joueur

		int MonstreCount;
		for (MonstreCount = 0; MonstreCount < listMonstre.size(); MonstreCount++)
		{
			if (listMonstre[MonstreCount].Vie > 0)
			{
				if (listMonstre[MonstreCount].enCollisionX == true) { listMonstre[MonstreCount].velX = 0; }
				if (listMonstre[MonstreCount].enCollisionY == true) { listMonstre[MonstreCount].velY = 0; }

				listMonstre[MonstreCount].x += listMonstre[MonstreCount].velX;
				listMonstre[MonstreCount].y += listMonstre[MonstreCount].velY;

				DrawRect(listMonstre[MonstreCount].x, listMonstre[MonstreCount].y, 2 * largeurPixel, 2 * largeurPixel, olc::Pixel(255, 255, 255)); //On déssine le monstre

				listMonstre[MonstreCount].enCollisionX = false;
				listMonstre[MonstreCount].enCollisionY = false;
			}
			else listMonstre.erase(listMonstre.begin() + MonstreCount);

		}

		if (MAP1_finie==true && j.demiCoeur > 0 && LeBoss.Vie > 0) {
			DrawRect(LeBoss.x, LeBoss.y, 5 * largeurPixel, 5 * largeurPixel, olc::Pixel(255, 255, 255)); //On déssine le Boss
			afficherVieBoss(LeBoss);
		}
		return true;
	}

};

//Dans le main, on lance simplement le projet game, qui correspond à lancer tout les éléments précédents

int main() {

	Projet game;
	if (game.Construct(largeurEcran, hauteurEcran, 1, 1)) {
		game.Start();
	}


	return true;
}

//On définit les différents types de collision, c'est à dire quand elles doivent renvoyer true ou false, par exemple quand la balle passe à moins d'un pixel d'un bloc

//Colision entre le perso et les monstres

bool Projet::MonstreColiJoueur(Joueur Jo, Monstre Mo)
{

	if (Jo.xPosJoueur + 50 >= Mo.x && Jo.xPosJoueur <= Mo.x + largeurPixel && repoptime < clock() - 500) {
		if (Jo.yPosJoueur + 50 >= Mo.y && Jo.yPosJoueur < Mo.y + largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

//Colision entre les bloc et les balles

bool Projet::BlocColiBullet(Bloc Bl, Bullet Bu)
{

	if (Bu.x + 1 >= Bl.x && Bu.x <= Bl.x + largeurPixel) {
		if (Bu.y + 1 >= Bl.y && Bu.y < Bl.y + largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

//Collision entre un type de bloc (celui qui change de couleur) et les balles

bool Projet::PixelJauneColiBullet(PixelJaune Bl, Bullet Bu)
{

	if (Bu.x + 10 >= Bl.x && Bu.x <= Bl.x + largeurPixel) {
		if (Bu.y + 10 >= Bl.y && Bu.y < Bl.y + largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

//Collision entre le joueur et les blocs

std::vector<bool> Projet::BlocColiJoueur(Bloc Bl, Joueur j) //2*largeurPixel = w et h du joueur ; largeurPixel = w et h d'un bloc
{
	bool xCol = false;
	bool yCol = false;

	float x = j.xPosJoueur + j.velXJoueur;
	float y = j.yPosJoueur + j.velYJoueur;

	if (Bl.x < x + 2 * largeurPixel && Bl.x + largeurPixel > x)
	{
		if (y + 2 * largeurPixel < Bl.y || y > Bl.y + largeurPixel) xCol = false;
		else xCol = true;
	}
	if (Bl.y < y + 2 * largeurPixel && Bl.y + largeurPixel > y)
	{
		if (x + 2 * largeurPixel < Bl.x || x > Bl.x + largeurPixel) yCol = false;
		else yCol = true;
	}

	std::vector<bool> l;
	l.push_back(xCol);
	l.push_back(yCol);

	return l;


}

bool Projet::BlocColiJoueurBool(Bloc Bl, Joueur J)
{
	if (Bl.x + 2*largeurPixel >= J.xPosJoueur && Bl.x <= J.xPosJoueur + 2 * largeurPixel) {
		if (Bl.y + 2*largeurPixel >= J.yPosJoueur && Bl.y < J.yPosJoueur + 2 * largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;
}



bool Projet::BulletColiMonstre(Bullet Bu, Monstre M) {

	if (Bu.x + 10 >= M.x && Bu.x <= M.x + 2 * largeurPixel) {
		if (Bu.y + 10 >= M.y && Bu.y < M.y + 2 * largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

bool Projet::BulletColiJoueur(Bullet Bu, Joueur J)
{
	if (Bu.x + 10 >= J.xPosJoueur && Bu.x <= J.xPosJoueur + 2 * largeurPixel) {
		if (Bu.y + 10 >= J.yPosJoueur && Bu.y < J.yPosJoueur + 2 * largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;
}

std::vector<double> Projet::GetCoordsVelMonstre(Monstre M, Joueur J) {

	double vx;
	double vy;

	std::vector<double> coords;

	if ((M.x - J.xPosJoueur) < 0)
	{
		vx = cos(atan((J.yPosJoueur - M.y) / (J.xPosJoueur - M.x))) * 2.0;	//* fElapsedTime;
		vy = sin(atan((J.yPosJoueur - M.y) / (J.xPosJoueur - M.x))) * 2.0;	//* fElapsedTime;
	}
	else if ((M.x - J.xPosJoueur) > 0)
	{
		vx = -cos(atan((J.yPosJoueur - M.y) / (J.xPosJoueur - M.x))) * 2.0;	//* fElapsedTime;
		vy = -sin(atan((J.yPosJoueur - M.y) / (J.xPosJoueur - M.x))) * 2.0;	//* fElapsedTime;
	}

	else if ((M.x - J.xPosJoueur) == 0)
	{
		vx = 0;
		vy = 0;
	}

	coords.push_back(vx);
	coords.push_back(vy);

	return coords;
}

std::vector<bool> Projet::BlocColiMonstre(Bloc Bl, Monstre M)
{
	bool xCol = false;
	bool yCol = false;

	float x = M.x + M.velX;
	float y = M.y + M.velY;

	if (Bl.x < x + 2 * largeurPixel && Bl.x + largeurPixel > x)
	{
		if (y + 2 * largeurPixel < Bl.y || y > Bl.y + largeurPixel) xCol = false;
		else xCol = true;
	}
	if (Bl.y < y + 2 * largeurPixel && Bl.y + largeurPixel > y)
	{
		if (x + 2 * largeurPixel < Bl.x || x > Bl.x + largeurPixel) yCol = false;
		else yCol = true;
	}
	std::vector<bool> l;
	l.push_back(xCol);
	l.push_back(yCol);

	return l;
}

std::vector<bool> Projet::MonstreColiMonstre(Monstre M1, std::vector<Monstre> ListMontre)
{
	bool xCol = false;
	bool yCol = false;



	return std::vector<bool>();
}

Projet::Bullet Projet::BossShootsBullet(Boss B, Joueur J)
{
	Bullet o;
	o.x = B.x + 4 * largeurPixel;
	o.y = B.y + 4 * largeurPixel;
	o.sprite = new olc::Sprite("Sprites/Bullet.png");

	if ((o.x - J.xPosJoueur) < 0)
	{
		o.vx = cos(atan((J.yPosJoueur - o.y) / (J.xPosJoueur - o.x))) * o.Vitesse; //* fElapsedTime;
		o.vy = sin(atan((J.yPosJoueur - o.y) / (J.xPosJoueur - o.x))) * o.Vitesse; //* fElapsedTime;
	}
	else if ((o.x - J.xPosJoueur == 0)) {
		o.vx = 0;
		o.vy = 0;
	}
	else if ((o.x - J.xPosJoueur > 0))
	{
		o.vx = -cos(atan((J.yPosJoueur - o.y) / (J.xPosJoueur - o.x))) * o.Vitesse; //* fElapsedTime;
		o.vy = -sin(atan((J.yPosJoueur - o.y) / (J.xPosJoueur - o.x))) * o.Vitesse; //* fElapsedTime;
	}

	return o;

}

Projet::Monstre Projet::BossSpawnMonstre(Boss B, Joueur J)
{
	Monstre M;

	float diffx = B.x - J.xPosJoueur;
	float diffy = B.y - J.yPosJoueur;

	if (diffx > 2) { M.x = B.x + rand() % int(diffx / 2) - 50; }          //+ (rand() % (B.x - J.xPosJoueur) / 2) - 50; 
	else if (-2 > diffx > 2) { M.x = 0; }
	else if (diffx < -2) { M.x = B.x - (rand() % int(diffx / 2)) + 50; }

	if (diffy > 2) { M.y = B.y + (rand() % int(diffy / 2)) - 50; }
	else if (-2 > diffy > 2) { M.y = 0; }
	else if (diffy < -2) { M.y = B.y - (rand() % int(diffy / 2)) + 50; }


	return M;
}

void Projet::afficherVieBoss(Boss B)
{
	int n = ceil(B.Vie / 3);
	int i = 0;
	int l = 0;
	for (l = 0; l < B.Vie_MAX / 3; l++)
	{
		DrawRect(450 + l, 960, 1, 20, olc::Pixel(255, 0, 0));
	}
	for (i = 0; i < n; i++)
	{
		DrawRect(450 + i, 960, 1, 20, olc::Pixel(0, 255, 0));
	}

}

bool Projet::BulletColiBoss(Bullet Bu, Boss Bo)
{
	if (Bu.x + 10 >= Bo.x && Bu.x <= Bo.x + 5 * largeurPixel) {
		if (Bu.y + 10 >= Bo.y && Bu.y < Bo.y + 5 * largeurPixel)
		{
			return true;
		}
		else return false;
	}
	else return false;

}

