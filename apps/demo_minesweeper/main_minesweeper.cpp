#include <Grapic.h>

#define CORRECTION

using namespace grapic;
using namespace std;

const int WIN_DIM_X = 300;
const int WIN_DIM_Y = 300;
const int TGX=9;
const int TGY=9;
const int MINES=5;

struct CaseGrille
{
    int contenu;
    bool decouverte;
    bool drapeau;
};

struct Jeu
{
    CaseGrille g[100][100];
	int dx,dy;
	int nb_mines;
	int nb_cases_a_decouvrir;
	bool perdu;
	Image im_mine;
	Image im_drapeau;
};


void placeMines(Jeu& jj)
{
	int x,y;
    for(int num_bombe=0;num_bombe<MINES;num_bombe++)
    {
        do{
            x=rand()%TGY+1;       //le 1 pour ce décaler d'une colonne car on veut pas les contour
            y=rand()%TGX+1;
        }
        while(jj.g[x][y].contenu!=0);
        jj.g[x][y].contenu=(-1);
    }

}


void compteMinesVoisines(Jeu& jj)
{
    for(int i=0;i<TGY+1;i++)                       //1 pour pas remplir les contour
        for(int j=0;j<TGX+1;j++)
            if(jj.g[i][j].contenu!=-1)                //si ya une bombe(-1) on change rien
                for(int k=i-1;k<=i+1;k++)
                    for(int l=j-1;l<=j+1;l++)      //double for parcour les contours
                        if(jj.g[k][l].contenu==-1)
                            jj.g[i][j].contenu++;
}



void init(Jeu& jj)
{
	jj.dx = 10;
	jj.dy = 10;
	jj.im_mine = image("data/mines/mine.png");
	jj.im_drapeau = image("data/mines/flag.png");
	int i,j;
	for(i=0;i<jj.dx;++i)
        for(j=0;j<jj.dy;++j)
        {
            jj.g[i][j].contenu = 0;
            jj.g[i][j].decouverte = false;
            jj.g[i][j].drapeau = false;
        }
    jj.nb_mines = 4;
    jj.nb_cases_a_decouvrir = jj.dx * jj.dy - jj.nb_mines;
    jj.perdu = false;

    placeMines(jj);
    compteMinesVoisines(jj);
}

int caseVersEcranX(const Jeu& jj, int x)
{
    return int(x * WIN_DIM_X/jj.dx);
}

int caseVersEcranY(const Jeu& jj, int y)
{
    return int(y * WIN_DIM_Y/jj.dy);
}

int ecranVersCaseX(const Jeu& jj, int x)
{
    return int(x / (WIN_DIM_X/jj.dx));
}

int ecranVersCaseY(const Jeu& jj, int y)
{
    return int(y / (WIN_DIM_Y/jj.dy));
}



void draw(Jeu& jj)
{
	int i,j;
	backgroundColor( 100, 80, 200, 255 );

	color(255, 0, 0);
	grid(0, 0, WIN_DIM_X-1, WIN_DIM_Y-1, jj.dx, jj.dy);

	color(255, 255, 0);
	fontSize(15);

	for(i=0;i<jj.dx+1;i++)
    {
        for(j=0;j<jj.dy+1;j++)
        {
            if(jj.g[i][j].decouverte)
            {
                if(jj.g[i][j].contenu==-1)
                {
                    image_draw( jj.im_mine, caseVersEcranX(jj,i) + 5, caseVersEcranY(jj,j) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
                }
                else
                    print( caseVersEcranX(jj,i)+10, caseVersEcranY(jj,j)+5, jj.g[i][j].contenu);
            }
        }
    }

	if (isMousePressed(SDL_BUTTON_LEFT))
	{
		int x, y, i, j;
		mousePos(x, y);

        i = ecranVersCaseX(jj,x);
        j = ecranVersCaseY(jj,y);
        jj.g[i][j].decouverte=true;
        if(jj.g[i][j].contenu==-1)
        {
            print(120,150,"PERDU");
            image_draw(jj.im_drapeau,70,110,120,120);
        }

    }


}



int main(int , char ** )
{
    srand(time(NULL));
    Jeu jeu;
    bool stop=false;
	winInit("Minesweeper", WIN_DIM_X, WIN_DIM_Y);
	setKeyRepeatMode(false);
	init(jeu);
	while( !stop )
    {
        backgroundColor( 100, 150, 250 );
        winClear();
        draw(jeu);
        stop = winDisplay();
    }
    winQuit();
	return 0;
}

