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

int geraMapa(){
    //definição dos limites do mapa
    for(int i=0;i<45;i++){
        for(int j=0;j<189;j++){
            if(i==0 || j==0) Mapa[i][j]='#';
            if(i==44 || j==188) Mapa[i][j]='#';
        }
    }
}

int compactaMapa(Mapa [][]){
    for (int i=0;i<45;i++){
        for (int j=0;j<189;j++){
            int cont=0;
            if(Mapa[i-1][j-1]='#') cont ++;
            if(Mapa[i-1][j]='#') cont ++;
            if(Mapa[i-1][j+1]='#') cont ++;
            if(Mapa[i][j-1]='#') cont ++;
            if(Mapa[i][j]='#') cont ++;
            if(Mapa[i][j+1]='#') cont ++;
            if(Mapa[i+1][j-1]='#') cont ++;
            if(Mapa[i+1][j]='#') cont ++;
            if(Mapa[i+1][j+1]='#') cont ++;

            if(cont>=5) Mapa[i][j]='#';
            else Mapa[i][j]=' ';
        }
    }
    return 0;
}



