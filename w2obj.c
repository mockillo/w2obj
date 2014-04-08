#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int numRows, numCols, nodata, numberOfVertices, numberOfTriangles;
double llx, lly, cellsize;
unsigned long filesize;

struct vertex {
    float x, y, z, valid;
};

struct triangle {
    int v1, v2, v3;
};

struct vertex* vertices;
struct triangle* triangles;

int rc2index(int row, int col){
    return (col * numRows) + row;
}

void createTriangle(int i0, int i1, int i2, int i3){
    //Check shortest diagonal
    vertex v0 = vertices[i0];
    v0.
    //triangle t;
    //t.v1 = i0;
}

int readFile(char* filename){
    numberOfVertices = 0;
    FILE* file;

    file = fopen(filename, "r");

    if(!file){
        printf("Unable to open file %s\n", filename);
        return 0;
    }

    fread(&numRows, sizeof(int), 1, file);
    fread(&numCols, sizeof(int), 1, file);
    fread(&llx, sizeof(double), 1 ,file);
    fread(&lly, sizeof(double), 1, file);
    fread(&cellsize, sizeof(double), 1, file);
    fread(&nodata, sizeof(int), 1, file);

    printf("Reading %s.\n", filename);
    printf("n: %d, m: %d, cellsize: %f\n", numRows, numCols, cellsize);
    printf("llx: %f, lly: %f, nodata: %d\n", llx, lly, nodata);

    float temp;
    vertices = (struct vertex*) malloc(sizeof(struct vertex) * (numRows*numCols));

    int i, j;
    for(i = 0; i < numRows; i++){
        for(j = 0; j < numCols; j++){
            fread(&temp, sizeof(float), 1, file);
            struct vertex v;
            v.valid = 0;

            if(temp > nodata){
                v.valid = 1;
                v.x = (float) i * cellsize;
                v.z = (float) j * cellsize;
                v.y = temp;
            }

            vertices[numberOfVertices++] = v;
        }
    }


    triangles = (struct triangle*) malloc(sizeof(struct triangle) * (numRows*numCols));

    for(i = 0; i < numRows - 1; i++){
        for(j = 0; j < numCols - 1; j++){
            createTriangle(rc2index(i,j), rc2index(i+1, j), rc2index(i, j+1), rc2index(i+1, j+1));
        }
    }

    fclose(file);

    printf("Read %d vertices from %s\n", numberOfVertices, filename);

    return 1;
}

int writeFile(char* filename){
    FILE* file;

    file = fopen(filename, "w");

    if(!file){
        printf("Error opening %s, for some reason.\n", filename);
        return 0;
    }

    fprintf(file, "mtllib ./%s.mtl\n", filename);

    int i;

    for(i = 0; i < numberOfVertices; i++){
        fprintf(file, "v %f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }

    fclose(file);

    printf("Wrote vertices to %s\n", filename);

    return 1;
}

void clean(){
    if(vertices)
        free(vertices);
}

int main(int argc, char** argv){
    if(argc < 4 || argc > 4){
        printf("Invalid parameters.\nUsage: w2obj <inputfile> <texturefile> <outputfile>\n");
        exit(1);
    }

    if(!readFile(argv[1])){
        clean();
        return 1;
    }

    if(!writeFile(argv[3])){
        clean();
        return 1;
    }

    clean();

    return 0;
}

