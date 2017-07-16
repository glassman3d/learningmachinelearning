/*
Learning Machine Learning - Al Jabr Project Vex Functions
Include into your vex nodes or wrangles to access the functions
Author Sam Swift-Glasman
sam@learning-machine-learning.com
*/

#ifndef __al_jabr__
#define __al_jabr__

function vector4 [] aj_prim_shape(int num_sides; float shape_scale;)
{
    vector4 pts[];
    vector4 startP = {0,0,1,1};
    float theta;
    matrix R;
    vector rotation = {0,0,0};
    vector position = {0,0,0};
    vector scale = {1,1,1};

    for(int i=0; i<=num_sides; i++)
    {
      theta = i * (360/num_sides);
      rotation.y = theta;
      R = maketransform( 0,0,position, rotation, scale, position );
      append(pts, startP*R);
    }

    return pts;
}

function vector4 [] aj_transform_pts(vector4 pts[]; matrix transformation)
{
    vector4 trans_pts[];
    foreach(vector4 pt; pts)
    {

        append(trans_pts, pt*transformation);

    }
    return trans_pts;
}

function void aj_add_prim_edges(vector4 pts[]; int shape_id; vector color)
{
    int tmp;
    int pr = addprim(geoself(),"poly");

    foreach(vector4 pt; pts)
    {
        tmp = addpoint(geoself(),vector(pt));
        addvertex(geoself(),pr,tmp);
    }
    addattrib(geoself(), "prim", "shape_id", shape_id);
    addattrib(geoself(), "point", "Cd", color);

}

function void aj_add_edges(vector4 pts[]; int num_sides)
{

    float iterations = len(pts) / (num_sides/2);
    //printf("%g", iterations );

    vector4 pt;
    for( int i=0; i<iterations; i++ )
    {

       vector4 shape_pts[];
       for(int j=0;j<=num_sides;j++)
       {
           append(shape_pts, pop(pts));
       }

       aj_add_prim_edges(shape_pts, i, {1,1,1});
    }
}

function vector4 [] al_jabr_layer()
{
    vector4 pts[];
    vector4 shape_pts[] = aj_prim_shape(5,1);
    vector position = {0,0,0};
    vector rotation = {0,0,0};
    vector pivot = {0,0,0};
    vector scale = {1,1,1};
    matrix tmp_matrix;
    for(int i=0;i<10;i++)
    {

            position.x = i*1;
            rotation.y = i*90;
            tmp_matrix = maketransform( 0, 0, position, rotation, scale, pivot );
            append(pts, aj_transform_pts(shape_pts,tmp_matrix));

    }
    return pts;
}

function void aj_al_jabr()
{

  // initial layer with offset
  vector4 pts[] = al_jabr_layer();
  matrix trans = ident();
  translate( trans,{-4.52447,0,0});
  pts = aj_transform_pts(pts,trans);
  matrix sc = ident();
  scale( sc, {1,1,-1});
  vector4 rot_pts[] = aj_transform_pts(pts,sc);
  append(pts,rot_pts);


  // multiple layers
  vector4 final_pts[];
  vector m_trans = {0,0,0};
  vector m_rot = {0,0,0};

  for(int i=0; i<8; i++ )
  {
    m_trans.z = i;
    m_rot.y = 180*i;
    //printf("trans %g",m_trans);
    //printf("rot %g",m_rot);
    matrix m_all = maketransform( 1, 0, m_trans, m_rot, {1,1,1}, m_trans );
    //printf("R %g",m_all);
    append( final_pts, aj_transform_pts(pts,m_all) );
  }


  // add edges
  aj_add_edges(final_pts,5);

}

#endif
