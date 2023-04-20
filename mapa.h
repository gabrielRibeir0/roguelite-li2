typedef struct terr{
    int ambiente;//mundo aberto 0, caverna 1
    int chao;//pedra 0, lava 1
}Terreno;

typedef struct obs{
    int muro;//0 sem muro,1 com muro
}Obstaculo; 

typedef struct mapa {
    struct Terreno;
    struct Obstaculo;
}Mapa;

int geraMapa()

int compactaMapa(Mapa [][]){
    for (int i=0;i<45;i++){
        for (int j=0;j<189;j++){
            if(Mapa[i][j]=='#' && Mapa[i][j+1]==' ' && Mapa[i][j+2]=='#'){
                Mapa[i][j+1]='#';
                Mapa[i][j+2]=' ';
            }
        }
    }
}

