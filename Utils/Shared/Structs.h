
// Definition of struct variables
typedef struct{
  WINDOW* win;
  int life;
  int color;
}LIFE;

typedef struct{
  WINDOW* win;

  char* name;

  int dim_rows,dim_cols;
  char** shape;
  
  int color1,color2;
  
  int x,y;
  bool pos;
  
  int maxBullet;
  int nbrBullet;
  int *BulletX;
  int *BulletY;

  LIFE life;
}SHIP;

typedef struct{
  char* ip;
  int port;
}SERVER;


// Externalisation of the global variables
extern SHIP ship1;
extern SHIP ship2;
extern SERVER serv_info;
