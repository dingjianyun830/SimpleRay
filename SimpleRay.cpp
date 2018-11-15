#include "vec3.h"
#include "camera.h"
#include "texture.h"
#include "ray.h"
#include "perlin.h" 
#include "Tex_CheckerBoard.h"

CheckerBoard *CBTex1 = new CheckerBoard();// generate texture
CheckerBoard *CBTex2 = new CheckerBoard();// generate texture

bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	double dt = dot(uv, n);
	double discriminant = 1.0 - ni_over_nt*(1 - dt*dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt*(uv - dt*n) - sqrt(discriminant)*n;
		return true;
	}
	else
	{
		return false;
	}
}

vec3 color(const vec3& p1)
{
	vec3 col;
	if (p1.e[0] < -1.0 || p1.e[1] < -1.0)
	{
		col[0] = 0;
		col[1] = 0;
		col[2] = 0;
	}
	else if (p1.e[0] > 1.0 || p1.e[1] > 1.0)
	{
		col[0] = 0;
		col[1] = 0;
		col[2] = 0;
	}
	else
	{
		vec3 p2 = 128 * (p1 + vec3(1.0,1.0,0.0));
		col[0] = CBTex1->GetPixel(int(p2.e[0]), int(p2.e[1]), 0);
		col[1] = CBTex1->GetPixel(int(p2.e[0]), int(p2.e[1]), 1);
		col[2] = CBTex1->GetPixel(int(p2.e[0]), int(p2.e[1]), 2);
	}

	return col;
}

int main()
{
	// set the plate size
	int w = 256;
	int h = 256;
	FILE *fp = fopen("test.ppm", "wb");
	fprintf(fp, "P6\n%d %d\n255\n", w, h);

	// generate texture
	CBTex1->GenTex1();
	CBTex2->GenTex2();

	// set the 3D location of camera and plate
	vec3 lower_left_corner(-1.0, -1.0, -1.0);
	vec3 horizontal(2.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	bool m_bRefract = true;
	for (int j = 0; j <h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			double u = double(i) / double(w);
			double v = double(j) / double(h);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical, 0);
			double ni_over_nt = 1/1.33;
			vec3 refracted = vec3(0.0,0.0,0.0);
			vec3 normal = vec3(0, 0, -0.5);
			vec3 p1 = vec3(0.0, 0.0, 0.0);	
			//double h = ((double)rand() / (RAND_MAX));
			double h = 0.5;
			if (m_bRefract)
			{
				refract(r.point_at_parameter(h), normal, ni_over_nt, refracted);
				ray r_out(r.point_at_parameter(h), refracted, 0.0);
				p1 = r_out.point_at_parameter(1 - h);
			}
			else
			{
				p1 = r.point_at_parameter(1);
			}
			
			perlin noise;
			//vec3 col = noise.noise(p1)*color(p1);
			vec3 surface = vec3(CBTex2->GetPixel(i, j, 0), CBTex2->GetPixel(i, j, 1), CBTex2->GetPixel(i, j, 2));
			vec3 col = noise.turb(p1)*(0.5*color(p1)+0.5*surface);

			static unsigned char MyColor[4];
			MyColor[0] = int(255.99*col[0]);
			MyColor[1] = int(255.99*col[1]);
			MyColor[2] = int(255.99*col[2]);
			MyColor[3] = int(255*0.5+128*0.5);
			fwrite(MyColor, 1, 4, fp);
		}
	}
	fclose(fp);

	return 1;
}