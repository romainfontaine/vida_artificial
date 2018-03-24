float am = 0;
float scalex = 1;
float scaley = 1;
PVector[] p = new PVector[4];
void d() {
  for (int i = 0; i<4; i++) {
    float fx = p[i].x;
    float fy = p[i].y; 
    float tx = p[(i+1)%4].x;
    float ty = p[(i+1)%4].y;
    fy+=(fy<0?-1:1)*(exp(fx*am)-1);
    ty+=(ty<0?-1:1)*(exp(tx*am)-1);
    fx*=scalex;
    tx*=scalex;
    fy*=scaley;
    ty*=scaley;
    line(fx*100, fy*100, tx*100, ty*100);
  }
}
void mouseClicked() {
  am = random(-.4,.4);
  scalex = random(.5, 1.5);
  scaley = random(.5, 1.5);
  println(am, scalex);
}

void setup() {
  size(800, 800);


  p[0] = new PVector(0, .5);
  p[1] = new PVector(1, .5);
  p[2] = new PVector(1, -.5);
  p[3] = new PVector(0, -.5);
}
void draw() {

  background(0);
  stroke(255);
  translate(300, 350);
  d();
}
