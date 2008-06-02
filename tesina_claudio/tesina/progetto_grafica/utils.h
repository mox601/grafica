/*
Determina il punto sulla superficie di una semisfera determinata dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!) e il raggio radius,
memorizzando il risultato in dest.
*/
void getPointOnSemiSphere(float dest[3],float radius,float u,float v);

/*
Calcola il prodotto vettoriale tra u e v, memorizzandolo in out.
*/
void crossProd(GLfloat out[3], GLfloat u[3], GLfloat v[3]);

/*
Normalizza il vettore passato come parametro portandone il modulo a 1.
*/
void normalize(GLfloat dest[3]);

/*
Disegna un quadrilatero specificando le texCoords, dati i suoi 4 vertici;
è limitato ad alcune tipologie semplici di quadrilateri, e segue l'ordine
dei vertici in input per la generazione del poligono.
Funziona anche con i triangoli (così sembra!), specificando due vertici uguali.
*/
void verTex(
	char* name,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float x4, float y4, float z4
	);

/*
Disegna un poliedro specificando le texCoords, dati i vertici delle facce superiore e inferiore
(che devono essere necessariamente rettangoli! oppure triangoli, facendo coincidere due vertici);
gli spigoli disegnati saranno (up1,down1) (up2,down2) (up3,down3) (up4,down4), quindi bisogna 
specificare la faccia inferiore con orientamento OPPOSTO (la funzione correggerà automaticamente).
Valgono le stesse semplificazioni della funzione verTex.
*/
void poliTex(
	char* name,
	float xup1, float yup1, float zup1,
	float xup2, float yup2, float zup2,
	float xup3, float yup3, float zup3,
	float xup4, float yup4, float zup4,
	float xdown1, float ydown1, float zdown1,
	float xdown2, float ydown2, float zdown2,
	float xdown3, float ydown3, float zdown3,
	float xdown4, float ydown4, float zdown4
	);

/*
Disegna la superficie laterale di un'arcata.
estensione arcata (X,Y): 	[0,2*radius] * [0,radius]
*/
void drawSuperficieArcata(float radius, int semicircleSteps);

/*
Disegna la differenza tra una superficie quadrata e una circonferenza.
estensione quadrato bucato (X,Y): 	[0,base]*[0,base]
*/
void drawQuadratoBucato(float base, int quarterCircleSteps);

/*
Disegna un cilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
estensione cilindro (X,Y,Z): 		[-radius,radius] * [0,h] * [-radius,radius]
*/
void drawCilindro(float radius, float h, int steps);

/*
Disegna un semicilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
il semicindro ha le normali rivolte verso l'esterno.
estensione semicilindro (X,Y,Z): 	[-radius,radius] * [0,h] * [0,radius]
*/
void drawSemiCilindroOutside(float radius, float h, int steps);

/*
Disegna un semicilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
il semicindro ha le normali rivolte verso l'interno.
estensione semicilindro (X,Y,Z): 	[-radius,radius] * [0,h] * [0,radius]
*/
void drawSemiCilindroInside(float radius, float h, int steps);

/*
Disegna una sfera di raggio pari a radius e precisione dettata da nx e ny.
estensione sfera (X,Y,Z):		[-radius,+radius] * [-radius,+radius] * [-radius,+radius]
*/
void drawSphere(float radius,int nx,int ny);

/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
void drawSphereQuarter(float radius,int nx,int ny);

/*
Modifica il rapporto per determinare il texture mapping.
*/
void setTextureScaling(float ratio);

//i parametri dicono tutto.. la base è centrata nell'origine
/*
Disegna un mezzo cono di raggio pari a raggio, altezza pari ad altezza (ma va'??) e precisione dettata da slices.
estensione (X,Y,Z):		[-raggio,+raggio] * [0,+altezza] * [0,+raggio]
*/
void drawMezzoCono(GLfloat raggio, GLfloat altezza, GLfloat spessore, GLint slices);


float pointDistance(float p1[3], float p2[3]);

/*
Disegna un mezzo toro di raggio pari a radius, raggio della sezione pari a section e precisione dettata dai params.
(in pratica il semicerchio superiore!)
estensione (X,Y,Z):		[-radius-section,+raggio+section] * [0,+altezza+section] * [-section,+section]
*/
void drawSemiTorus(float radius,float section,int nx,int ny);

/*
Disegna un quarto di toro di raggio pari a radius, raggio della sezione pari a section e precisione dettata dai params.
(in pratica il pezzo superiore a sinistra!)
estensione (X,Y,Z):		[-radius-section,0] * [0,+altezza+section] * [-section,+section]
*/
void drawTorusQuarter(float radius,float section,int nx,int ny);


void drawCilindroChiuso(float radius, float h, int steps);



/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
Il quarto di sfera ha le normali di faccia rivolte verso l'esterno.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
void drawSphereQuarterOutside(float radius,int nx,int ny);


/*
Disegna un cilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
Il cilindro è illuminato all'interno!
estensione cilindro (X,Y,Z): 		[-radius,radius] * [0,h] * [-radius,radius]
*/
void drawCilindroInside(float radius, float h, int steps);


/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
Il quarto di sfera ha le normali di faccia rivolte verso l'interno.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
void drawSphereQuarterInside(float radius,int nx,int ny);

float random01();

void randomColor(float color[4]);


void drawArco(float radius, float width, int semicircleSteps);


/*
Disegna una semisfera di raggio radius e precisione steps.
estensione su X,Y,Z: [-radius,+radius]*[0,0]*[0,+radius]
L'origine coincide con il centro degli assi
*/
void drawSemisfera(float radius, int steps);

void drawFiammella();

void drawFiammellaLampadario();
