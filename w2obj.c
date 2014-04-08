#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n, m, nodata, numberOfVertices;
double llx, lly, cellsize;
unsigned long filesize;

struct v3 {
    float x, y, z;
};

struct v3* vertices;

int readFile(char* filename){
    numberOfVertices = 0;
    FILE* file;

    file = fopen(filename, "r");

    if(!file){
        printf("Unable to open file %s\n", filename);
        return 0;
    }

    fread(&n, sizeof(int), 1, file);
    fread(&m, sizeof(int), 1, file);
    fread(&llx, sizeof(double), 1 ,file);
    fread(&lly, sizeof(double), 1, file);
    fread(&cellsize, sizeof(double), 1, file);
    fread(&nodata, sizeof(int), 1, file);

    printf("Reading %s.\n", filename);
    printf("n: %d, m: %d, cellsize: %f\n", n, m, cellsize);
    printf("llx: %f, lly: %f, nodata: %d\n", llx, lly, nodata);

    float temp;
    vertices = (struct v3*) malloc(sizeof(struct v3) * (n+m));

    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            fread(&temp, sizeof(float), 1, file);

            if(temp > nodata){
                struct v3 v;
                v.x = (float) i * cellsize;
                v.y = (float) j * cellsize;
                v.z = temp;
                vertices[numberOfVertices++] = v;
            }
        }
    }

    fclose(file);

    printf("Read %d vertices from %s\n", numberOfVertices, filename);

    return 1;
}

int writeFile(char* filename){
    FILE* file;

    file = fopen("filename", "w");

    //fprintf(f, )

    return 1;
}

int main(int argc, char** argv){
    if(argc < 4 || argc > 4){
        printf("Invalid parameters.\nUsage: w2obj <inputfile> <texturefile> <outputfile>\n");
        exit(1);
    }

    if(!readFile(argv[1])){
        return 1;
    }

    free(vertices);

    return 0;
}

