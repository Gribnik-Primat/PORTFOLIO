/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 08.06.2013
 */

#version 400

out vec4 OutColor;

in vec4 UseColor;

/* Main function */
void main( void )
{
  OutColor = vec4(0.0, 1.0, 0.0, 1.0);
} /* End of 'main' function */

/* End of 'a.frag' file */
