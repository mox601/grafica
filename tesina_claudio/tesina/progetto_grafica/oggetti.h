/* La panchina viene disegnata avendo il suo vertice in basso a sinistra nell'origine.
Si estenderà perciò in larghezza(lungo X), in altezza(lungo Y) e in profondità lungo i valori negativi di Z 
La panchina è profonda 7.5
La panchina è alta 11
La panchina è larga 25
*/
void drawPanchina();

/*_____________________________________________________________________________________________________________________________________*/

/* Disegna l'altare ponendo il vertice in basso a sinistra della base nell'origine.
	Dimensioni base: 30x; 12y; -18z
*/
void drawAltare();

/*_____________________________________________________________________________________________________________________________________*/

/* Occorre disegnarla rigorosamente nello stesso piano(ovvero senza alcuna traslazione, rotazione, scalamento) dell'altare,
	solo così può poggiarsi esattamente sopra
*/ 
void drawTovaglia();

/*_____________________________________________________________________________________________________________________________________*/

/* Base altare pone la base maggiore nel punto 0, 0, 0 corrispondente al suo vertice basso a sinistra */
void drawBaseAltare(float x, float y, float z);

/*_____________________________________________________________________________________________________________________________________*/

/*Disegna la candela centrata nell'origine degli assi e intorno il contenitore: tutto parametrico
	dobbiamo passargli:  	- il raggio del contenitore esterno
				- l'altezza del contenitore esterno
				- la precisione del contenitore esterno
			  	- il raggio della candela interna
				- l'altezza della candela interna
				- la precisione della candela interna
*/
void drawCero(float radius, float altezza, int steps);

/*_____________________________________________________________________________________________________________________________________*/

/*Disegna la croce con il vertice in basso a sinistra della base nell'origine*/
void drawCroce(float xperp, float yperp);

/*_____________________________________________________________________________________________________________________________________*/

/*Il quadro viene disegnato con coordinata del vertice della cornice in basso a sinistra nell'origine
	Ogni quadro ha una cornice con dimensione predefinita */
void drawQuadro(float dimx, float dimy, float sporg_corn, int ref_texture);

/*_____________________________________________________________________________________________________________________________________*/

/* Il lampione deve essere appeso fuori, sulle pareti della chiesa
	Le sue dimensioni si estenderanno di: 
			-  dimx_base_sup sull'asse x
		 	-  dimy+0.2+1/4dimy sull'asse y
			-  -dimx_base_sup sull'asse z
	La base inferiore del lampione è centrata nell'origine
*/
void drawLampione(float dimx_base_inf, float dimx_base_sup, float dimy);

/*_____________________________________________________________________________________________________________________________________*/

void drawCandelabro(float altezza, float larghezza);

/*_____________________________________________________________________________________________________________________________________*/

/*La colonna è centrata nell'origine
	La dimensione della parte superiore si determina automaticamente a seconda delle dimensioni passate come parametro ed è:
	dimx*dimx(lato*lato) ovvero un quadrato pari alla dimensione della base della colonna
	Occorre passare la texture per la colonna e quella per le basi
*/
void drawColonna(float dimx, float altezza, float radius, int texture_colonna, int texture_basi);

/*_____________________________________________________________________________________________________________________________________*/

/* Lampadario con dimensione fissa, si estende secondo queste dimensioni:
		-  44x(ovvero 4 metri e 4decimetri)
		-  10y (più lunghezza della corda che può essere scelta)
		-  44z(ovvero 4 metri e 4decimetri)
   Il lampadario è centrato nell'origine
*/	
void drawLampadario();

/*_____________________________________________________________________________________________________________________________________*/

/* L'acquasantiera è centrata nell'origine e ha raggio radius e si estende sui tre assi così:
	-  da -1.04*radius a 1.04*radius sull'asse x
	-  da -1.04*radius a 0.04*radius sull'asse y
	-  da -1.04*radius a 0.04*radius sull'asse z
*/
void drawAcquasantiera(float radius);

void drawCandela(float larghezza, float altezza, int precisione);

void drawSchieraPanchine(int n);

void drawTavoloCeri(float larghezza, float altezza, float profondita, int ceri);
