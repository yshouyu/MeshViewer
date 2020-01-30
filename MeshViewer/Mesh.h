


#define M_PI 3.1415926

#define MESH_NONE     (0)           
#define MESH_FLAT     (1 << 0)       
#define MESH_SMOOTH   (1 << 1)
#define GLM_MATERIAL (1 << 4)    

typedef struct Triangle_s {
  int Vind[3];        
  int Nind[3];         
  int Tind[3];          
  int idx;               
} Triangle_t;

typedef struct Group_s {
  char*             tag;  
  int            trinums; 
  int*           tri;    

  struct Group_s* next;
} Group_t;



typedef struct CMesh_s {
  char*    filepath;            /* path to this mesh */


  int   vert_nums;         /* number of verts in mesh */
  float* verts;            /* array of verts  */

  int   nor_nums;          /* number of normals in mesh */
  float* normals;             /* array of normals */


  int   fn_nums;       /* number of f_normals in mesh */
  float* f_normals;          /* array of f_normals */

  int   trinums;    /* number of tri in mesh */
  Triangle_t* tri;       /* array of tri */


  int       gp_nums;       /* number of gps in mesh */
  Group_t*      gps;          /* linked list of gps */

  float pos[3];          /* pos of the mesh */
  float color[3];

  float* objShaderData; /*shader data*/
  float* colorShaderData;
  float* normalShaderData;

} CMesh;

float meshunt(CMesh* mesh);
void  meshScale(CMesh* mesh, float scale);
void  meshfn(CMesh* mesh);
void  meshvn(CMesh* mesh, float angle);
CMesh*  meshOBJLoad(char* filename);
void convertToShaderData(CMesh* mesh);


