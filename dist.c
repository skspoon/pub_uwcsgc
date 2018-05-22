#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Task to solve:
// Given a point and a list of line segments in a 3D space,
// calculate the point on a line segment which is closest to the point provided.

// Assumption:
// the "list" of line segment is an English term to mean a logical collection,
// and does not necessarily mean the collection of line segments in a List data structure.
#define LINE_SEQMENTS 500

// a point in the 3D space
typedef struct pnt {
  double x; double y; double z;
} PNT;

// a line segment in 3D with the two end points points
typedef struct line {
  PNT pt1;
  PNT pt2;
} LINE;

// a 3D vector
typedef struct vect {
  double x; double y; double z;
} VECT;


// pt1 is the source, pt2 is the destination
double copy_pnt (PNT pt1, PNT * pt2) {
  pt2->x = pt1.x;
  pt2->y = pt1.y;
  pt2->z = pt1.z;
}

// distance between two points
double dist_pts (PNT pa, PNT pb) {
  double dx = (pb.x - pa.x);
  double dy = (pb.y - pa.y);
  double dz = (pb.z - pa.z);
  return sqrt(dx*dx + dy*dy + dz*dz);
}

// define a vector from two points, from point 1 (pt1) to point 2 (pt2).
VECT pts2vect (PNT pt1, PNT pt2, VECT * vc) {
  vc->x = pt2.x - pt1.x;
  vc->y = pt2.y - pt1.y;
  vc->z = pt2.z - pt1.z;
  return (*vc);
}

// define a vector from the end points of a line seqment, from point 1 to point 2.
VECT line2vect (LINE ll, VECT * vc) {
  vc->x = ll.pt2.x - ll.pt1.x;
  vc->y = ll.pt2.y - ll.pt1.y;
  vc->z = ll.pt2.z - ll.pt1.z;
  return (*vc);
}

// for debug, can ignore
void show_line(LINE ln) {
  printf("show line: (%f, %f, %f), (%f, %f, %f);\n"
	 , ln.pt1.x, ln.pt1.y, ln.pt1.z
	 , ln.pt2.x, ln.pt2.y, ln.pt2.z);
}

// for debug, can ignore
void show_vect(VECT * vc) {
  printf("show vector: (%f, %f, %f);\n", vc->x, vc->y, vc->z);
}

// magnitude of a vector
double mag_vect (VECT vc) {
  return (sqrt(vc.x * vc.x + vc.y * vc.y + vc.z * vc.z));
}

// projection of vector va onto vector vb
double dot_prd (VECT va, VECT vb) {
  double dd = (va.x * vb.x + va.y * vb.y + va.z * vb.z);
  show_vect(&va);
  show_vect(&vb);
  printf("dot_prod: %f;\n", dd);
  return(dd);
  // return(va.x * vb.x + va.y * vb.y + va.z * vb.z);
}

VECT vect_prd (VECT va, VECT vb, VECT * vd) {

  vd->x =  (va.y * vb.z - vb.y * va.z);
  vd->y = -(va.x * vb.z - vb.x * va.z);
  vd->z =  (va.x * vb.y + vb.x * va.y);

  // double dd = (va.y * vb.z - vb.y * va.z) - (va.x * vb.z - vb.x * va.z) + (va.x * vb.y - vb.x * va.y);
  show_vect(&va);
  show_vect(&vb);
  printf("vect_prod: ");
  show_vect(vd);

  return(*vd);
  // return((va.y * vb.z - vb.y * va.z) - (va.x * vb.z - vb.x * va.z) + (va.x * vb.y + vb.x * va.y));
}

void pnt_init(PNT *pt, double x, double y, double z) {
  pt->x = x;
  pt->y = y;
  pt->z = z;
}
void line_init(LINE *ln, double x1, double y1, double z1, double x2, double y2, double z2) {
  ln->pt1.x = x1;
  ln->pt1.y = y1;
  ln->pt1.z = z1;

  ln->pt2.x = x2;
  ln->pt2.y = y2;
  ln->pt2.z = z2;
}

LINE line_segs[LINE_SEQMENTS];

// add the contens for a line into the global line segment array
int line_segs_add(LINE ln, int cnt) {

  // initialize the line_segs array elements.
  line_init(&line_segs[cnt], ln.pt1.x, ln.pt1.y, ln.pt1.z, ln.pt2.x, ln.pt2.y, ln.pt2.z);

  cnt++;
  
  return cnt;
}


// // distance of a point to a vector
// double dist_pt_vect (PNT pt, VECT vc) {
//   LINE ll;
//   ll.pt1.x = pt.x;  ll.pt1.y = pt.y;  ll.pt1.z = pt.z;
//   ll.pt2.x = vc.
// }

// Shortest distance from a point to a line segment:
// case one: one of the end points is the closest point;
// case two: there is a perpendicular distance onto one point at the line segment
double dist_pt_line (PNT pt, LINE ll) {
  ///  LINE ln1; LINE ln2; LINE llr;
  VECT vt1; VECT vt2;
  VECT vt3; VECT vt4;
  double dd1; double dd2;
  double mag_ll;

  // show_line(ll);

  /// // can form a trangle (3 lines) from the 3 points:
  /// // ln1 is the line from end point 1 of ll to pt
  /// copy_pnt(ll.pt1, &ln1.pt1);
  /// copy_pnt(pt,     &ln1.pt2);
  /// // ln2 is the line from end point 2 of ll to pt
  /// copy_pnt(ll.pt2, &ln2.pt1);
  /// copy_pnt(pt,     &ln2.pt2);

  // direction of vt1 is from ll.pt1 to the pt
  pts2vect(ll.pt1, pt, &vt1);
 
  // direction of vt2 is from ll.pt2 to the pt
  pts2vect(ll.pt2, pt, &vt2);

  // direction of vt3 is of the line, from ll.pt1 to ll.pt2
  line2vect(ll, &vt3);


  // // direction of vt4 is from ll.pt2 to ll.pt1, reverse of vt3
  // copy_pnt(ll.pt1, &llr.pt2);
  // copy_pnt(ll.pt2, &llr.pt1);
  // line2vect(llr, &vt4);

  dd1 = dot_prd(vt1, vt3);
  dd2 = dot_prd(vt2, vt3);

  if ((dd1 >= 0 && dd2 >= 0) || (dd1 <= 0 && dd2 <= 0)) {
    // The perpendicular projection to the vector vt3
    // is not "inside" the line segment, so
    // one of the end points is the closest point.

    dd1 = dist_pts(pt, ll.pt1);
    dd2 = dist_pts(pt, ll.pt2);

    if (dd1 < dd2)
      return dd1;
    else
      return dd2;
  }
  else { // ((dd1 > 0 && dd2 < 0) || (dd1 < 0 && dd2 > 0))

    // the perpendicular distance from the point to the line segment
    mag_ll = dist_pts(ll.pt1, ll.pt2);
    vt4 = vect_prd(vt1, vt3, &vt4);

    return (mag_vect(vt4) / mag_ll);
  }
}

// The main is for simple testing with command line inputs
int main (int ac, char ** av) {

  double min1 = 0;
  double min2 = 0;
  PNT pnt;
  double x, y, z;  x = y = z = 0;

  LINE lin;
  int ii;

  pnt_init(&pnt, 0, 0, 0);
  if (ac > 3) {
    x = atof(av[1]);
    y = atof(av[2]);
    z = atof(av[3]);
    pnt_init(&pnt, x, y, z);
    printf("Point: (%f, %f, %f)\n", x, y, z);
  }

  if (ac > 9) {
    line_init(&lin, atof(av[4]), atof(av[5]), atof(av[6]), atof(av[7]), atof(av[8]), atof(av[9]));
  }
  int lines = 0;
  lines = line_segs_add(lin, lines);

  for (ii = 0; ii < lines; ii++) {

    show_line(line_segs[ii]);
    min2 = dist_pt_line(pnt, line_segs[ii]);

    if (ii == 0)
      min1 = min2;

    if (min2 < min1)
      min1 = min2;

    printf("\nThe line segment [%d] has the shortest distance of (%f) to the point (%f, %f, %f).\n"
	   , ii, min1, pnt.x, pnt.y, pnt.z);
  }
}
