#include <math.h>
#include <stdlib.h>
#include "3DDriver.h"

void C3DDriver::ResetMatrix(t_matrix m)
{
	if (m > mtx_count) return;
	C3DTransform& t = matrix[m];
	memset(&t, 0, sizeof(t));
	t.sx = t.sy = t.sz = t.qw = 1;
}

void C3DDriver::RotateMatrix(t_matrix m, float x, float y, float z, float a)
{
	if (m > mtx_count) return;
	C3DTransform& t = matrix[m];

	float w = (float)(sin(a*0.5) / sqrt(x*x + y*y + z*z));
	x *= w;
	y *= w;
	z *= w;
	w = (float)cos(a*0.5);

	float rw = w*t.qw - x*t.qx - y*t.qy - z*t.qz;
	float rx = w*t.qx + x*t.qw + y*t.qz - z*t.qy;
	float ry = w*t.qy - x*t.qz + y*t.qw + z*t.qx;
	float rz = w*t.qz + x*t.qy - y*t.qx + z*t.qw;
	t.qw = rw;
	t.qx = rx;
	t.qy = ry;
	t.qz = rz;

	float t1 =  w*x;
	float t2 =  w*y;
	float t3 =  w*z;
	float t4 = -x*x;
	float t5 =  x*y;
	float t6 =  x*z;
	float t7 = -y*y;
	float t8 =  y*z;
	float t9 = -z*z;

	rx = 2*((t7 + t9)*t.vx + (t5 - t3)*t.vy + (t2 + t6)*t.vz);
	ry = 2*((t3 + t5)*t.vx + (t4 + t9)*t.vy + (t8 - t1)*t.vz);
	rz = 2*((t6 - t2)*t.vx + (t1 + t8)*t.vy + (t4 + t7)*t.vz);
	t.vx += rx;
	t.vy += ry;
	t.vz += rz;
}

void C3DDriver::AxisAngle(t_matrix m, float *x, float *y, float *z, float *a)
{
	if (m > mtx_count) return;
	C3DTransform& t = matrix[m];

	float n = (float)sqrt(t.qx*t.qx + t.qy*t.qy + t.qz*t.qz);
	*a = 2 * (float)atan2(n, t.qw);

	if (fabs(*a) < 1.0e-5 || fabs(*a - PI) < 1.0e-5)
	{
		*x = 1;
		*y = 0;
		*z = 0;
	}
	else
	{
		n = 1 / (1 - t.qw*t.qw);
		*x = t.qx * n;
		*y = t.qy * n;
		*z = t.qz * n;
	}

	while (*a > PI)
		*a -= 2*PI;
	while (*a < -PI)
		*a += 2*PI;
}

void C3DDriver::TranslateMatrix(t_matrix m, float dx, float dy, float dz)
{
	if (m > mtx_count) return;
	C3DTransform& t = matrix[m];
	t.vx += dx / t.sx;
	t.vy += dy / t.sy;
	t.vz += dz / t.sz;
}

void C3DDriver::ScaleMatrix(t_matrix m, float sx, float sy, float sz)
{
	if (m > mtx_count) return;
	C3DTransform& t = matrix[m];
	t.sx *= sx;
	t.sy *= sy;
	t.sz *= sz;
}

void C3DDriver::Cleanup()
{
	while (!vertex_array.empty())
	{
		DeleteVertexArray(vertex_array.back());
		vertex_array.pop_back();
	}

	while (!texture.empty())
	{
		DeleteTexture(texture.back());
		texture.pop_back();
	}
}
