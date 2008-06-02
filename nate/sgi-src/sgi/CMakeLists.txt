PROJECT( sgi )

SET( PROGRAMS area nii rotate texture maiden null starfield voronoi multiview
qix strip )

FIND_PACKAGE( GLUT )
FIND_PACKAGE( OPENGL )

FOREACH( program ${PROGRAMS} )
  ADD_EXECUTABLE( ${program} ${program}.c )
  TARGET_LINK_LIBRARIES( ${program} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} )
ENDFOREACH( program )
