#include <estado.h>
#include <mapa.h>

int geramapa(){
    //definição dos limites do mapa
    for(int i=0;i<45;i++){
        for(int j=0;j<189;j++){
            if(i==0 || j==0) mapa[i][j]='#';
            if(i==44 || j==188) mapa[i][j]='#';
        }
    }
}

int compactamapa(CASA mapa[][10]){
    for (int i=0;i<45;i++){
        for (int j=0;j<189;j++){
            int cont=0;
            if(mapa[i-1][j-1].obs) cont ++;
            if(mapa[i-1][j].obs == MURO) cont ++;
            if(mapa[i-1][j+1].obs == MURO) cont ++;
            if(mapa[i][j-1].obs == MURO) cont ++;
            if(mapa[i][j].obs == MURO) cont ++;
            if(mapa[i][j+1].obs == MURO) cont ++;
            if(mapa[i+1][j-1].obs == MURO) cont ++;
            if(mapa[i+1][j].obs == MURO) cont ++;
            if(mapa[i+1][j+1].obs == MURO) cont ++;

            if(cont>=5) mapa[i][j].obs = MURO;
            else mapa[i][j].obs = VAZIO;
        }
    }
    return 0;
}
