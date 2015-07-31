#include "ParadigmaTemperature.h"
#include <stdio.h>
#include <math.h>

using namespace Paradigma;

ParadigmaTemperature::operator string() const 
{ 
	char Buffer[14]; 
	sprintf(Buffer, "%d,%1d C", m_temp/10, m_temp%10); 

	return Buffer;
}

ParadigmaTemperature::operator float() const 
{ 
	return ((float)m_temp)/10.0f; 
}

ParadigmaTemperature& ParadigmaTemperature::operator=(float f) 
{ 
	m_temp = (int)ceil(f*10.0f); 
}

ParadigmaTemperature& ParadigmaTemperature::operator=(ParadigmaTemperature &p) 
{ 
	m_temp = p.m_temp; 
}

