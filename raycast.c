#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parser.c"
#include "ppmrw.c"

typedef struct {
  int kind; // 0 = camera, 1 = sphere, 2 = plane
  double color[3];
  union {
    struct {
      double width;
      double height;
    } camera;
    struct {
      double position[3];
      double radius;
    } sphere;
    struct {
      double position[3];
      double normal[3];
    } plane;
  };
} Object;

static inline double sqr(double v) {
  return v*v;
}

static inline void nomarlize(double* v) {
  double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
  v[0] /= len;
  v[1] /= len;
  v[2] /= len;
}

double sphereIntersection(double* Ro, double* Rd, double* Center, double r) {
  // x = Rox + Rdx*t
  // y = Roy + Rdy*t
  // z = Roz + Rdz*t
  // (x - Centerx)^2 + (y - Centery)^2 + (z - Centerz)^2 = r^2
  //
  // Then, (Rox + Rdx*t - Centerx)^2 + (Roy + Rdy*t - Centery)^2
  // + (Roz + Rdz*t - Centery)^2 = r^2
  //
  // Then, Rox^2 + Rdx^2*t^2 + Centerx^2 + 2*Rox*Rdx*t - 2*Rox*Centerx - 2*Rdx*t*Centerx
  // + Roy^2 + Rdy^2*t^2 + Centery^2 + 2*Roy*Rdy*t - 2*Roy*Centery - 2*Rdy*Centery
  // + Roz^2 + Rdz^2*t^2 + Centerz^2 + 2*Roz*Rdz*t - 2*Roz*Centerz - 2*Rdz*Centerz
  //
  // Then, t^2(Rdx^2 + Rdy^2 + Rdz^2) +
  // t*(2*Rox*Rdx + 2*TRoz*Rdz + 2*Roy*Rdy) +
  // Rox^2 + Centerx^2 + Roy^2 + Centery^2 + Roz^2 + Centerz^2 -
  // 2*Rox*Centerx - 2*Rdx*Centerx - 2*Roy*Centery - 2*Rdy*Centery - 2*Roz*Centerz - 2*Rdz*Centerz
  // - r^2 = 0
  double a = sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[2]);
  double b = 2*(Ro[0]*Rd[0] + Ro[1]*Rd[1] + Ro[2]*Rd[2]);
  double c = sqr(Ro[0]) + sqr(Ro[1]) + sqr(Ro[2]) + sqr(Center[0]) +
             sqr(Center[1]) + sqr(Center[2]) - 2*(Ro[0]*Center[0] + Rd[0]*Center[0]
             + Ro[1]*Center[1] + Rd[1]*Center[1] + Ro[2]*Center[2] + Rd[2]*Center[2])
             - sqr(r);
  double det = sqr(b)) - 4*a*c;
  if (det < 0) return -1;
  det = sqrt(det);

  double t0 = (-b - det) / (2*a);
  if (t0 > 0) return t0;
  double t1 = (-b + det) / (2*a);
  if (t1 > 0) return t1;

  return -1;
}

double planeIntersection(double* Ro, double* Rd, double* position double normal) {
  // A(X0 + Xd * t) + B(Y0 + Yd * t) + (Z0 + Zd * t) + D = 0
  // A(Rox + Rdx*t) + B(Roy + Rdy*t) + C(Roz + Rdz*t) + D = 0
  // it could also be written as:
  // A(Rox + Rdx*t - positionx) + B(Roy + Rdy*t - postiony) + C(Roz + Rdz*t - positionz) = 0
  // t(A*Rdx + B*Rdy + C*Rdz) + (A*Rox + B*Roy + C*Roz - A*positionx - B*positiony - C*postionz) = 0
  // t = - (A*Rox + B*Roy + C*Roz - A*positionx - B*positiony - C*positionz) / (A*Rdx + B*Rdy + C*Rdz)
  double t = - (normal[0]*Ro[0] + normal[1]*Ro[1] + normal[2]*Ro[2] - normal[0]*position[0]
                - normal[1]*position[1] - normal[2]*position[2]) / (normal[0]*Rd[0]
                + normal[1]*Rd[1] + normal[2]*Rd[2]);

  if (t > 0) return t;
  return -1;
}


// this function write the image into ppm file
void writeFile(char* filename){
  Object** objects = malloc(sizeof(Object*)*2);
}

int main(int argc, char **argv){
  if (argc != 5){
    fprintf(stderr, "Error: incorrect format('raycast width height input.json output.ppm')");
    return (1);
  }
  char *width = argv[1];
  char *height = argv[2];
  char *inputFilename = argv[3];
  char *outputFilename = argv[4];
  readScene(inputFilename);

  writeFile(outputFilename);
}
