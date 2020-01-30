#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mesh.h"
#include "math.h"
#include<time.h>

#define TRI(x) (mesh->tri[(x)])

Group_t* getgp(CMesh* mesh, char* tag)
{
    Group_t* gp;
    gp = mesh->gps;
    while(gp) 
	{
        if (!strcmp(tag, gp->tag))
            break;
        gp = gp->next;
    }
    
    return gp;
}

Group_t* addgp(CMesh* mesh, char* tag)
{
    Group_t* gp;
    gp = getgp(mesh, tag);
    if (!gp) 
	{
        gp = (Group_t*)malloc(sizeof(Group_t));
        gp->tag = strdup(tag);

        gp->trinums = 0;
        gp->tri = NULL;
        gp->next = mesh->gps;
        mesh->gps = gp;
        mesh->gp_nums++;
    }
    return gp;
}

void getdata(CMesh* mesh, FILE* file) 
{
    GLuint vert_nums;        /* number of verts in mesh */
    GLuint nor_nums;         /* number of normals in mesh */
 
    GLuint trinums;       /* number of tri in mesh */
    Group_t* gp;           /* current gp */
    int v, n, t;
    char buffer[128];//读取文本字符串
    
  
    gp = addgp(mesh, "null");//？？
    
    vert_nums =0;
	nor_nums =0;
	trinums = 0;
    while(fscanf(file, "%s", buffer) != EOF)
	{
        switch(buffer[0])
		{
            case 'v':       /*vert*/
                fgets(buffer, sizeof(buffer), file);//没用
                vert_nums++;
                break;
            break;
            case 'f':               /* face */
                v = n = t = 0;
                fscanf(file, "%s", buffer);
            
                if (sscanf(buffer, "%d/%d", &v, &t) == 2) //没用
				{
                    /* v/t */
                    fscanf(file, "%d/%d", &v, &t);
                    fscanf(file, "%d/%d", &v, &t);
                    trinums++;
                    gp->trinums++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) 
					{
                        trinums++;
                        gp->trinums++;
                    }
                } else {
                    /* v */
                    fscanf(file, "%d", &v);
                    fscanf(file, "%d", &v);
                    trinums++;
                    gp->trinums++;
                    while(fscanf(file, "%d", &v) > 0)
					{
                        trinums++;
                        gp->trinums++;
                    }
                }
                break;
                
           
        }
  }
  
  mesh->vert_nums  = vert_nums;
  mesh->nor_nums   = nor_nums;

  mesh->trinums = trinums;

  gp = mesh->gps;
  while(gp)
  {
      gp->tri = (int*)malloc(sizeof(int) * gp->trinums);
      gp->trinums = 0;
      gp = gp->next;
  }
}


void dealdata(CMesh* mesh, FILE* file) 
{
    GLuint vert_nums;        /* number of verts in mesh */
    GLuint nor_nums;         /* number of normals in mesh */

    GLuint trinums;       /* number of tri in mesh */
    float* verts;         /* array of verts  */
    float* normals;          /* array of normals */

    Group_t* gp;           /* current gp pointer */

    int v, t;
    char buffer[128];

    verts   = mesh->verts;
    normals = mesh->normals;

    gp   = mesh->gps;
    

    vert_nums =1;
	nor_nums = 1;
    trinums = 0;

    while(fscanf(file, "%s", buffer) != EOF) {
        switch(buffer[0]) 
		{
        case 'v':
		{
                /* vertex */
            fscanf(file, "%f %f %f", &verts[3 * vert_nums + 0], &verts[3 * vert_nums + 1], &verts[3 * vert_nums + 2]);
            vert_nums++;//临时索引
            break;
		}
		case 'f':       
		{
			/* face */
            v  = t = 0;
            fscanf(file, "%s", buffer);
            if (sscanf(buffer, "%d/%d", &v, &t) == 2) 
			{
                /* v/t */
                TRI(trinums).Vind[0] = v < 0 ? v + vert_nums : v;
                fscanf(file, "%d/%d", &v, &t);
                TRI(trinums).Vind[1] = v < 0 ? v + vert_nums : v;
                fscanf(file, "%d/%d", &v, &t);
                TRI(trinums).Vind[2] = v < 0 ? v + vert_nums : v;
                gp->tri[gp->trinums++] = trinums;
                trinums++;
                while(fscanf(file, "%d/%d", &v, &t) > 0)
				{
                    TRI(trinums).Vind[0] = TRI(trinums-1).Vind[0];
                    TRI(trinums).Tind[0] = TRI(trinums-1).Tind[0];
                    TRI(trinums).Vind[1] = TRI(trinums-1).Vind[2];
                    TRI(trinums).Tind[1] = TRI(trinums-1).Tind[2];
                    TRI(trinums).Vind[2] = v < 0 ? v + vert_nums : v;
                    gp->tri[gp->trinums++] = trinums;
                    trinums++;
                }
            } 
			else 
			{
                /* v */
                sscanf(buffer, "%d", &v);
				/*(mesh->tri[(trinums)])*/
                TRI(trinums).Vind[0] = v < 0 ? v + vert_nums : v;
                fscanf(file, "%d", &v);
                TRI(trinums).Vind[1] = v < 0 ? v + vert_nums : v;
                fscanf(file, "%d", &v);
                TRI(trinums).Vind[2] = v < 0 ? v + vert_nums : v;
                gp->tri[gp->trinums++] = trinums;
                trinums++;
                while(fscanf(file, "%d", &v) > 0)
				{
                    //TRI(trinums).Vind[0] = TRI(trinums-1).Vind[0];
                    //TRI(trinums).Vind[1] = TRI(trinums-1).Vind[2];
                    //TRI(trinums).Vind[2] = v < 0 ? v + vert_nums : v;
                    //gp->tri[gp->trinums++] = trinums;
                    //trinums++;
                }
            }
            break;
		}
		default:
			break;
		}
	}
}

float meshunt(CMesh* mesh)
{

    float x_max, x_min, y_max, y_min, z_max, z_min;
    float centerx, centery, centerz;
	float width, height, depth;
    float s;

    x_max = x_min = mesh->verts[3 + 0];
    y_max = y_min = mesh->verts[3 + 1];
    z_max = z_min = mesh->verts[3 + 2];
    for (int i = 1; i <= mesh->vert_nums; i++)
	{
        if (x_max < mesh->verts[3 * i + 0])
            x_max = mesh->verts[3 * i + 0];
        if (x_min > mesh->verts[3 * i + 0])
            x_min = mesh->verts[3 * i + 0];
        
        if (y_max < mesh->verts[3 * i + 1])
            y_max = mesh->verts[3 * i + 1];
        if (y_min > mesh->verts[3 * i + 1])
            y_min = mesh->verts[3 * i + 1];
        
        if (z_max < mesh->verts[3 * i + 2])
            z_max = mesh->verts[3 * i + 2];
        if (z_min > mesh->verts[3 * i + 2])
            z_min = mesh->verts[3 * i + 2];
    }
    
    /* calculate mesh width, height, and depth */
    width = getAbs(x_max) + getAbs(x_min);
    height = getAbs(y_max) + getAbs(y_min);
    depth = getAbs(z_max) + getAbs(z_min);
    
    /* calculate center of the mesh */
    centerx = (x_max + x_min) / 2.0;
    centery = (y_max + y_min) / 2.0;
    centerz = (z_max + z_min) / 2.0;
    
    s = 2.0 / getMax(getMax(width, height), depth);
    
    for (int i = 1; i <= mesh->vert_nums; i++) 
	{
        mesh->verts[3 * i + 0] -= centerx;
        mesh->verts[3 * i + 1] -= centery;
        mesh->verts[3 * i + 2] -= centerz;
        mesh->verts[3 * i + 0] *= s;
        mesh->verts[3 * i + 1] *= s;
        mesh->verts[3 * i + 2] *= s;
    }
    
    return s;
}

void meshScale(CMesh* mesh, float s)
{   
    for (int i = 1; i <= mesh->vert_nums; i++)
	{
        mesh->verts[3 * i + 0] *= s;
        mesh->verts[3 * i + 1] *= s;
        mesh->verts[3 * i + 2] *= s;
    }
}

void meshfn(CMesh* mesh)
{
    GLuint  i;
    float u[3];
    float v[3];
 
    if (mesh->f_normals)
        free(mesh->f_normals);
    
    /* allocate memory for the new facet normals */
    mesh->fn_nums = mesh->trinums;
    mesh->f_normals = (float*)malloc(sizeof(float) *3 * (mesh->fn_nums + 1));
    
    for (i = 0; i < mesh->trinums; i++) 
	{
        mesh->tri[i].idx = i+1;
        u[0] = mesh->verts[3 * TRI(i).Vind[1] + 0] -mesh->verts[3 * TRI(i).Vind[0] + 0];
        u[1] = mesh->verts[3 * TRI(i).Vind[1] + 1] -mesh->verts[3 * TRI(i).Vind[0] + 1];
        u[2] = mesh->verts[3 * TRI(i).Vind[1] + 2] -mesh->verts[3 * TRI(i).Vind[0] + 2];
        v[0] = mesh->verts[3 * TRI(i).Vind[2] + 0] - mesh->verts[3 * TRI(i).Vind[0] + 0];
        v[1] = mesh->verts[3 * TRI(i).Vind[2] + 1] -mesh->verts[3 * TRI(i).Vind[0] + 1];
        v[2] = mesh->verts[3 * TRI(i).Vind[2] + 2] - mesh->verts[3 * TRI(i).Vind[0] + 2];
        getCross(u, v, &mesh->f_normals[3 * (i+1)]);
        getNormalize(&mesh->f_normals[3 * (i+1)]);
    }
}

void meshvn(CMesh* mesh, float angle)
{
    Node_t* node;
    Node_t* tail;
    Node_t** pnode;
    float* normals;
    GLuint nor_nums;
    float range[3];
	float agl;
    float dot;
	GLuint avg;
    agl = cos(angle * M_PI / 180.0);
    if (mesh->normals)
        free(mesh->normals);
    /* allocate space for new normals */
    mesh->nor_nums = mesh->trinums * 3; /* 3 normals per triangle */
    mesh->normals = (float*)malloc(sizeof(float)* 3* (mesh->nor_nums+1));
    pnode = (Node_t**)malloc(sizeof(Node_t*) * (mesh->vert_nums + 1));
    for (int i = 1; i <= mesh->vert_nums; i++)
        pnode[i] = NULL;
    for (int i = 0; i < mesh->trinums; i++)
	{
        node = (Node_t*)malloc(sizeof(Node_t));
        node->idx = i;
        node->next  = pnode[TRI(i).Vind[0]];
        pnode[TRI(i).Vind[0]] = node;
        node = (Node_t*)malloc(sizeof(Node_t));
        node->idx = i;
        node->next  = pnode[TRI(i).Vind[1]];
        pnode[TRI(i).Vind[1]] = node;
        node = (Node_t*)malloc(sizeof(Node_t));
        node->idx = i;
        node->next  = pnode[TRI(i).Vind[2]];
        pnode[TRI(i).Vind[2]] = node;
    }
    
    /* calculate the range normal for each vertex */
    nor_nums = 1;
    for (int i = 1; i <= mesh->vert_nums; i++)
	{
        node = pnode[i];
        range[0] = 0.0; range[1] = 0.0; range[2] = 0.0;
        avg = 0;
        while (node)
		{
            dot = getDot(&mesh->f_normals[3 * TRI(node->idx).idx],&mesh->f_normals[3 * TRI(pnode[i]->idx).idx]);
            if (dot > agl)
			{
                node->avg = GL_TRUE;
                range[0] += mesh->f_normals[3 * TRI(node->idx).idx + 0];
                range[1] += mesh->f_normals[3 * TRI(node->idx).idx + 1];
                range[2] += mesh->f_normals[3 * TRI(node->idx).idx + 2];
                avg = 1;  
            } else {
                node->avg = GL_FALSE;
            }
            node = node->next;
        }
        
        if (avg) 
		{
            /* normalize the avg normal */
            getNormalize(range);
            
            /* add the normal to the vertex normals list */
            mesh->normals[3 * nor_nums + 0] = range[0];
            mesh->normals[3 * nor_nums + 1] = range[1];
            mesh->normals[3 * nor_nums + 2] = range[2];
            avg = nor_nums;
            nor_nums++;
        }
        /* set the normal of this vertex in each triangle it is in */
        node = pnode[i];
        while (node)
		{
            if (node->avg) 
			{
                /* if this node was avg, use the range normal */
                if (TRI(node->idx).Vind[0] == i)
                    TRI(node->idx).Nind[0] = avg;
                else if (TRI(node->idx).Vind[1] == i)
                    TRI(node->idx).Nind[1] = avg;
                else if (TRI(node->idx).Vind[2] == i)
                    TRI(node->idx).Nind[2] = avg;
            } 
			else
			{
          
                mesh->normals[3 * nor_nums + 0] = mesh->f_normals[3 * TRI(node->idx).idx + 0];
                mesh->normals[3 * nor_nums + 1] = mesh->f_normals[3 * TRI(node->idx).idx + 1];
                mesh->normals[3 * nor_nums + 2] = mesh->f_normals[3 * TRI(node->idx).idx + 2];
                if (TRI(node->idx).Vind[0] == i)
                    TRI(node->idx).Nind[0] = nor_nums;
                else if (TRI(node->idx).Vind[1] == i)
                    TRI(node->idx).Nind[1] = nor_nums;
                else if (TRI(node->idx).Vind[2] == i)
                    TRI(node->idx).Nind[2] = nor_nums;
                nor_nums++;
            }
            node = node->next;
        }
    }
    mesh->nor_nums = nor_nums - 1;

    for (int i = 1; i <= mesh->vert_nums; i++)
	{
        node = pnode[i];
        while (node) {
            tail = node;
            node = node->next;
            free(tail);
        }
    }
    free(pnode);
    
    normals = mesh->normals;

    mesh->normals = (float*)malloc(sizeof(float)* 3* (mesh->nor_nums+1));

    for (int i = 1; i <= mesh->nor_nums; i++)
	{
        mesh->normals[3 * i + 0] = normals[3 * i + 0];
        mesh->normals[3 * i + 1] = normals[3 * i + 1];
        mesh->normals[3 * i + 2] = normals[3 * i + 2];
    }
    free(normals);
}

CMesh* meshOBJLoad(char* filename)//default info
{
    CMesh* mesh;
    FILE* file;
    file = fopen(filename, "r");
    if (!file) 
	{
        exit(1);
    }
    
    /* allocate a new mesh */
    mesh = (CMesh*)malloc(sizeof(CMesh));
    mesh->filepath    = strdup(filename);
    mesh->vert_nums   = 0;
    mesh->verts    = NULL;//点的数据 x y z
    mesh->nor_nums    = 0;
    mesh->normals     = NULL;//点的法向量 
    mesh->fn_nums = 0;
    mesh->f_normals    = NULL;//面的法向量 都是指针
    mesh->trinums  = 0;
    mesh->tri       = NULL;//组成面的点的索引 哪些点组成的
	mesh->color[0]=0.8;
	mesh->color[1]=0.8;
	mesh->color[2]=0.8;
    mesh->gp_nums       = 0;//没用
    mesh->gps      = NULL;
    mesh->pos[0]   = 0.0;//没用
    mesh->pos[1]   = 0.0;
    mesh->pos[2]   = 0.0;
    
    getdata(mesh, file);
    /* allocate memory */
    mesh->verts = (float*)malloc(sizeof(float) * 3 * (mesh->vert_nums + 1));
    mesh->tri = (Triangle_t*)malloc(sizeof(Triangle_t) *mesh->trinums);
    if (mesh->nor_nums) 
	{
        mesh->normals = (float*)malloc(sizeof(float) *3 * (mesh->nor_nums + 1));
    }
    rewind(file);
    dealdata(mesh, file);
    fclose(file);
    return mesh;
}

void convertToShaderData(CMesh* mesh)
{
	static int i;
	static Group_t* gp;
	static Triangle_t* triangle;
	gp = mesh->gps;
	int idx = 0, c = 0;
	mesh->objShaderData = new float[gp->trinums * 12];//  坐标值  XYZW
	mesh->colorShaderData = new float[gp->trinums * 12];////颜色值 RGBA
	mesh->normalShaderData = new float[gp->trinums * 12];//法向量 UVW
	while (gp) 
	{
		for (i = 0; i < gp->trinums; i++) 
		{
			triangle = &TRI(gp->tri[i]);
			srand(unsigned(time(NULL)));
			{
				mesh->objShaderData[idx] = mesh->verts[3 * triangle->Vind[0]];
				mesh->objShaderData[idx + 1] = mesh->verts[3 * triangle->Vind[0] + 1];
				mesh->objShaderData[idx + 2] = mesh->verts[3 * triangle->Vind[0] + 2];
				mesh->objShaderData[idx + 3] = 1.0;
				mesh->normalShaderData[idx] = mesh->normals[3 * triangle->Nind[0]];
				mesh->normalShaderData[idx + 1] = mesh->normals[3 * triangle->Nind[0] + 1];
				mesh->normalShaderData[idx + 2] = mesh->normals[3 * triangle->Nind[0] + 2];
				mesh->normalShaderData[idx + 3] = 1.0;
				mesh->colorShaderData[idx] = 1.0;
				mesh->colorShaderData[idx + 1] = 1.0;
				mesh->colorShaderData[idx + 2] = 1.0;
				mesh->colorShaderData[idx + 3] = 1.0;
			}
			{
				mesh->objShaderData[idx + 4] = mesh->verts[3 * triangle->Vind[1]];
				mesh->objShaderData[idx + 5] = mesh->verts[3 * triangle->Vind[1] + 1];
				mesh->objShaderData[idx + 6] = mesh->verts[3 * triangle->Vind[1] + 2];
				mesh->objShaderData[idx + 7] = 1.0;
				mesh->normalShaderData[idx + 4] = mesh->normals[3 * triangle->Nind[1]];
				mesh->normalShaderData[idx + 5] = mesh->normals[3 * triangle->Nind[1]+1];
				mesh->normalShaderData[idx + 6] = mesh->normals[3 * triangle->Nind[1]+2];
				mesh->normalShaderData[idx + 7] = 1.0;
				mesh->colorShaderData[idx + 4] = 1.0;
				mesh->colorShaderData[idx + 5] = 1.0;
				mesh->colorShaderData[idx + 6] = 1.0;
				mesh->colorShaderData[idx + 7] = 1.0;
			}
			{
				mesh->objShaderData[idx + 8] = mesh->verts[3 * triangle->Vind[2]];
				mesh->objShaderData[idx + 9] = mesh->verts[3 * triangle->Vind[2] + 1];
				mesh->objShaderData[idx + 10] = mesh->verts[3 * triangle->Vind[2] + 2];
				mesh->objShaderData[idx + 11] = 1.0;
				mesh->normalShaderData[idx + 8] = mesh->normals[3 * triangle->Nind[2]];
				mesh->normalShaderData[idx + 9] = mesh->normals[3 * triangle->Nind[2] + 1];
				mesh->normalShaderData[idx + 10] = mesh->normals[3 * triangle->Nind[2] + 2];
				mesh->normalShaderData[idx + 11] = 1.0;
				mesh->colorShaderData[idx + 8] = 1.0;
				mesh->colorShaderData[idx + 9] = 1.0;
				mesh->colorShaderData[idx + 10] = 1.0;
				mesh->colorShaderData[idx + 11] = 1.0;
			}
			idx += 12;
		}
		gp = gp->next;
	}
}