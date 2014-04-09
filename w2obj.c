#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int numRows, numCols, nodata, numberOfVertices, numberOfTriangles;
double llx, lly, cellsize;
unsigned long filesize;

struct vertex {
    float x, y, z, valid, index, u, v;
};

struct triangle {
    int v1, v2, v3;
};

struct vertex minus(struct vertex l, struct vertex r){
    struct vertex temp;

    temp.x = l.x - r.x;
    temp.y = l.y - r.y;
    temp.z = l.z - r.z;
    temp.valid = 1;

    return temp;
}

float mag(struct vertex v){
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

struct vertex* vertices;
struct triangle* triangles;

int rc2index(int row, int col){
    return col * numRows + row;
}

void createTriangle(int i0, int i1, int i2, int i3){
    //Check shortest diagonal
    struct vertex v0 = vertices[i0];
    struct vertex v1 = vertices[i1];
    struct vertex v2 = vertices[i2];
    struct vertex v3 = vertices[i3];

    if(v0.valid != 1 || v1.valid != 1 || v2.valid != 1 || v3.valid != 1)
        return;

    struct triangle t1, t2;

    if(mag(minus(v0, v3)) < mag(minus(v1, v2))){
        t1.v1 = v0.index;
        t1.v2 = v2.index;
        t1.v3 = v1.index;

        t2.v1 = v2.index;
        t2.v2 = v3.index;
        t2.v3 = v1.index;
    } else {
        t1.v1 = v0.index;
        t1.v2 = v2.index;
        t1.v3 = v3.index;

        t2.v1 = v0.index;
        t2.v2 = v3.index;
        t2.v3 = v1.index;
    }

    triangles[numberOfTriangles++] = t1;
    triangles[numberOfTriangles++] = t2;
}

int readFile(char* filename){
    numberOfTriangles = 0;
    FILE* file;

    file = fopen(filename, "rb");

    if(!file){
        printf("Unable to open file %s\n", filename);
        return 0;
    }

    fread(&numCols, sizeof(int), 1, file);
    fread(&numRows, sizeof(int), 1, file);
    fread(&llx, sizeof(double), 1 ,file);
    fread(&lly, sizeof(double), 1, file);
    fread(&cellsize, sizeof(double), 1, file);
    fread(&nodata, sizeof(int), 1, file);

    printf("Reading %s.\n", filename);
    printf("n: %d, m: %d, cellsize: %f\n", numRows, numCols, cellsize);
    printf("llx: %f, lly: %f, nodata: %d\n", llx, lly, nodata);

    float temp;
    vertices = (struct vertex*) malloc(sizeof(struct vertex) * (numRows*numCols));

    int col, row;
    for(col = 0; col < numCols; col++){
        for(row = 0; row < numRows; row++){
            fread(&temp, sizeof(float), 1, file);
            struct vertex v;
            v.valid = 0;

            if(temp > nodata){
                v.valid = 1;
                v.x = (float) col * cellsize;
                v.z = (float) row * cellsize;
                v.y = temp;
                v.u = col / ((float) numCols - 1.0f);
                v.v = 1.0f - (row / (float) numRows - 1.0f);
                v.index = ++numberOfVertices;
            }

            vertices[col * numRows + row] = v;
        }
    }

    triangles = (struct triangle*) malloc(sizeof(struct triangle) * ((numRows*numCols)*2));

    for(col = 0; col < numCols - 1; col++){
        for(row = 0; row < numRows - 1; row++){
            createTriangle(rc2index(row,col), rc2index(row, col+1), rc2index(row+1, col), rc2index(row+1, col+1));
        }
    }

    fclose(file);

    printf("Read %d vertices from %s\n", numberOfVertices, filename);
    printf("Generated %d triangles from %s\n", numberOfTriangles, filename);

    return 1;
}

int writeFile(char* objname, char* textureFile){
    FILE* objfile;
    FILE* mtlfile;

    char* mtlname = (char*) malloc(sizeof(char) * strlen(textureFile) + 5);
    strcpy(mtlname, objname);
    strcat(mtlname, ".mtl");

    objfile = fopen(objname, "w");
    mtlfile = fopen(mtlname, "w");

    if(!objfile){
        printf("Error opening %s, for some reason.\n", objname);
        return 0;
    }

    fprintf(objfile, "mtllib ./%s.mtl\n", objname);

    int i;

    for(i = 0; i < numRows*numCols; i++){
        if(vertices[i].valid == 1){
            fprintf(objfile, "v %f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
            fprintf(objfile, "vt %f %f\n", vertices[i].u, vertices[i].v);
        }
    }

    fprintf(objfile, "usemtl bergen\n");
    for(i = 0; i < numberOfTriangles; i++){
        fprintf(objfile, "f %d/%d %d/%d %d/%d\n", triangles[i].v1, triangles[i].v1, triangles[i].v2, triangles[i].v2, triangles[i].v3, triangles[i].v3);

    }

    fprintf(mtlfile, "newmtl bergen\n");
    fprintf(mtlfile, "Ka 1.000 1.000 1.000\n");
    fprintf(mtlfile, "Kd 1.000 1.000 1.000\n");
    fprintf(mtlfile, "Ks 1.000 1.000 1.000\n");
    fprintf(mtlfile, "Tr 0.000\n");
    fprintf(mtlfile, "illum 2\n");
    fprintf(mtlfile, "map_Kd %s\n", textureFile);

    fclose(objfile);
    fclose(mtlfile);

    printf("Wrote data to %s and %s\n", objname, mtlname);
    free(mtlname);

    return 1;
}

void clean(){
    if(vertices)
        free(vertices);
    if(triangles)
        free(triangles);
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

    if(!writeFile(argv[3], argv[2])){
        clean();
        return 1;
    }

    clean();

    return 0;
}
