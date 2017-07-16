/*
Learning Machine Learning - Al Jabr Project Vex Functions
Include into your vex nodes or wrangles to access the functions
Author Sam Swift-Glasman
sam@learning-machine-learning.com
*/

#ifndef __al_jabr__
#define __al_jabr__

function vector [] aj_prim_shape(int num_sides; float shape_scale;)
{
    vector pts[];
    vector startP = {0,0,1};

    for(int i=0; i<=num_sides; i++)
    {
      float theta = i * (360/num_sides);
      vector rotation = {0,0,0};
      rotation.y = theta;
      matrix R = maketransform( 0 ,0, {0,0,0}, rotation, {1,1,1}, {0,0,0} );
      append(pts, startP*R);
    }

    return pts;
}

function vector [] aj_transform_pts(vector pts[]; matrix transformation)
{
    vector trans_pts[];
    foreach(vector pt; pts)
    {

        append(trans_pts, pt*transformation);

    }
    return trans_pts;
}

function void aj_add_prim_edges(vector pts[]; int shape_id; vector color)
{
    int tmp;
    int pr = addprim(geoself(),"poly");

    foreach(vector pt; pts)
    {
        tmp = addpoint(geoself(),vector(pt));
        addvertex(geoself(),pr,tmp);
    }
    addattrib(geoself(), "prim", "shape_id", shape_id);
    addattrib(geoself(), "point", "Cd", color);

}

function void aj_add_edges(vector pts[]; int num_sides)
{

    float iterations = len(pts) / (num_sides/2);

    vector pt;
    for( int i=0; i<iterations; i++ )
    {

       vector shape_pts[];
       for(int j=0;j<=num_sides;j++)
       {
           append(shape_pts, pop(pts));
       }

       aj_add_prim_edges(shape_pts, i, {1,1,1});
    }
}

function vector [] al_jabr_layer()
{
    vector pts[];
    vector shape_pts[] = aj_prim_shape(5,1);

    for(int i=0;i<10;i++)
    {
            vector position = {0,0,0};
            vector rotation = {0,0,0};
            position.x = i*1;
            rotation.y = i*90;
            matrix tmp_matrix = maketransform( 0, 0, position, rotation, {1,1,1}, {0,0,0} );
            append(pts, aj_transform_pts(shape_pts,tmp_matrix));

    }
    return pts;
}

function void aj_al_jabr()
{

  // initial layer with offset
  vector pts[] = al_jabr_layer();
  matrix trans = ident();
  translate( trans,{-4.52447,0,0});
  pts = aj_transform_pts(pts,trans);
  matrix sc = ident();
  scale( sc, {1,1,-1});
  vector rot_pts[] = aj_transform_pts(pts,sc);
  append(pts,rot_pts);


  // multiple layers
  vector final_pts[];
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
