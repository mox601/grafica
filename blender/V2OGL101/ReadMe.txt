D o c u m e n t a t i o n  f o r  V r m l 2 O G L . e x e  V 1 . 0
Copyright 1999 by Alexander Rohra. All rights Reserved.

(See ReadMe.html for an HTML version of this file.)

Contents:

I.    Introduction
II.   Archive Contents
III.  Disclaimer and Distribution Notice
IV.   Contact and Download Information
V.    Limitations and Notes
        General Information about the Source Code
        Terminology
        Header Files and Definition Order
        VRML Worlds and Camera & Light Definitions
        Model Correction/Appearance
        Major Shortcomings
        Data Precision
        Status/Error Codes
        Suggested Speed Improvements
        Recognized Nodes
        Unsupported Nodes
        Various Notes
VI.   Revision Notes


I. Introduction

    Vrml2OGL converts a VRML V1.0 file to OpenGL C source code and stores it
  in the following three output files:

  1) an OpenGL file holding the object drawing code contained in a C-callable
     function
  2) a header file containing the function prototype for the object drawing
     function contained in the OpenGL C file, a few #define statements and
     extern variable definitions
  3) a data file containing matrix as well material/color arrays (which are
     made public by the header file)

    The order of the OpenGL file will correspond to the order of the input
  .WRL file as closely as possible. The resulting code can be included in any
  source code project. In order to view a converted model, at least a light
  source and camera transformations need to be set up, prior to calling the
  object drawing function contained in the OpenGL file.

    The following is a list of command line switches recognized by Vrml2OGL
  and their explanations:
  -?   : show this list of command line switches and their descriptions
  -it  : include transformations such as translations, rotations, etc. if
         present in the input file (by DEFAULT xfrms are not included)
  -c   : compute the object center and subtract it from all object coordinates
  -cxV : subtract a value from each x coordinate, where V is any real number
         (0.0 by DEFAULT)
  -cyV : subtract a value from each y coordinate, where V is any real number
         (0.0 by DEFAULT)
  -czV : subtract a value from each z coordinate, where V is any real number
         (0.0 by DEFAULT)
  -rf  : invert the contents of the glFrontFace() statement; this inverts
         either the default face order or the face order specified by the
         input file for the current vertecis
  -rn  : revert all normals (generated or retrieved from the input file)
  -fV  : for each logical indentation use V amount of spaces, where 0<=V<8
  -ft  : for each logical indentation use one TAB (DEFAULT)
  -bV  : precede each line with an identation base of V amount of spaces,
         where 0<=V<8
  -bt  : precede each line with an identation base of one TAB (DEFAULT)
  -d   : prints progress info to stdout (helpful for debugging)

    Note: The -d option is only available if the program was compiled with
  _V_DEBUGMODE_ defined. The -? option will show if this was the case for the
  version of Vrml2OGL.exe that you are using.
  The -fV, -ft, -bV and -bt options only affect the appearance of the
  generated source code in the OpenGL file. The effect is purely "cosmetic"
  and has no influence on the code's functionality or performance.
  For additional information on some of the options listed above, please
  view the notes under "V. Limitations and Notes" below.

    The first command line argument not recognized by Vrml2OGL is considered
  the end of the list of command line switches (if any) and the name of the
  input file. The following three parameters (if present) are considered the
  file names of the OGL.c, Hdr.h and Dat.c files - in this order.

    Note: If there are less than 3 arguments left after the name of the input
  file, Vrml2OGL ignores these arguments and creates 3 default file names
  (base name of input file (without any non-aplha numeric characters) followed
  by OGL.c, Hdr.h and Dat.c, if the NOMODELID #define was set at compile time
  of Vrml2OGL.exe, the default output file names are simply OGL.c, Hdr.h,
  Dat.c (for more information on the NOMODELID #define see the revision notes
  for version 1.01 below).

II. Archive Contents

    The archive containing this file and Vrml2OGL.exe also contains the entire
  source code needed to compile Vrml2OGL.exe. Please read the Disclaimer and
  Distribution Notice below prior to using ANY of the supplied files.

    The following list shows the file names of all files supplied:

  ReadMe.html   (essentially the same as this file but in HTML format)
  ReadMe.txt	(this file)
  Vrml2OGL.exe
  src\FindNodes.cpp
  src\FindNodes.h
  src\GeometryUtils.cpp
  src\GeometryUtils.h
  src\MakeDebug.bat
  src\MakeRelease.bat
  src\SharedDefs.h
  src\Vrml2OGL.cpp
  src\Vrml2OGL.h
  src\Vrml2OGL.mak
  src\Vrml2OGLUtils.cpp
  src\Vrml2OGLUtils.h

  Note: Due to long filenames, please use WinZip to uncompress the archive.

III. Disclaimer and Distribution Notice

    This program, Vrml2OGL.exe and its accompanying source files, are
  distributed in the hope that they will be useful, but WITHOUT ANY WARRANTY;
  without even the implied warranty of MERCHANTABILITY, COMPLETENESS or
  FITNESS FOR ANY PARTICULAR PURPOSE.

    The author, Alexander Rohra, accepts no responsibility for any loss or
  damage to any person(s) and/or hardware or software, as a result of using
  the program Vrml2OGL.exe or its source code.

    Vrml2OGL.exe is released into public domain as "FREEWARE" and may be
  freely copied and modified provided the disclaimer mentioned above is
  maintained in effect for any released versions.

IV. Contact and Download Information

    For comments, questions and/or problems write to arohra@geocities.com.
  Please be aware that I might not be able to help you and/or publish any
  further versions of this program.

    In case you are missing any of the files listed under
  "II. Archive Contents" you can download the complete archive from
  http://www.geocities.com/SiliconValley/Program/3830 (updates will be posted
  here first), ftp://ftp.cdrom.com/pub/simtelnet/win95/graphics/v2ogl101.zip
  or ftp://avalon.viewpoint.com/pub/utils/converters/v2ogl101.zip.

V. Limitations and Notes

  * General Information about the Source Code

      Vrml2OGL.exe is compiled of multiple .cpp files. However, you will
    notice that the code contained in these source code modules is almost 100%
    C code. The only C++ feature used that required the .cpp file extension is
    "the free store" (new/delete). I prefered this over malloc() due to its
    ease of use and its flexibilty.

      On Win32 platforms, the source code can be easily compiled, either by
    using the included MakeRelease.bat or MakeDebug.bat batch files, or by
    creating a "Win32 Console" project, including all supplied .cpp files in
    it and compiling it from the IDE. If you decide to use the batch files,
    you should ensure that the directory paths specified in them, match the
    paths on your PC. By default, they assume a standard base path of
    C:\Program Files\DevStudio\VC.

      The source code should also be compatible with at least most compilers
    on various different UNIX platforms. It has been successfully tested by
    Scott Nelson on the following UNIX platforms: SGI IRIX 6.2 & 6.5,
    SunOS 4.1.4 & 5.5.1 (SOLARIS 1 & 2), DEC ALPHA OSD1 V4.0. You will need
    to rename all files with the file extension .cpp to .c. Some UNIX
    compilers will also require you to apply the dos2unix command on all .h
    and .c(pp) files. Depending on your compiler, you may then use a command
    line syntax similar to the following in order to perform the compilation:
    cc -DNOMODELID -o Vrml2OGL Vrml2OGLUtils.c FindNodes.c GeometryUtils.c
    Vrml2OGL.c -lm. Please note that the defintion of NOMODELID via
    -DNOMODELID should only be used, if you do not wish that the variable,
    defintion and function names generated by Vrml2OGL contain the input
    file's base name (see revision notes for version 1.01 below).

      Many functions are very similar to one another (even by name). This was
    done to cut down complexity in each function, thereby improving execution
    speed.

      Coordinate3 and Normal data is read into memory as doubles. Considering
    that each of these numbers are rounded to 4 digits after the decimal point
    when output, floats should actually be sufficient (this would have a
    slightly negative impact for Coordintate3 data when it is being used to
    calculate corresponding normals).

      Note: Please be aware that there are probably a number of more complete
    and efficient converters of this kind (such as VRML2GL.exe by Leigh McRae
    (C) 1997 by Now Enterprises) in existence. Also - as I discovered when I
    was 75% done with this project - there are professional code libraries
    available that can greatly assist in parsing and interpreting VRML files
    (see http://www.sdsc.edu/vrml/dvsoftware.html for more information on such
    libraries).

  * Terminology

      When you view the supplied source code you will notice the word
    "indentation" or "indent". I used this term to describe what is actually
    considered a parent node. In other words instead of using "parentNode" I
    used the far more generic and visually inspired term "indentation". You 
    will see this term in context with the following nodes: Separator, Group,
    Switch, LOD and TransformSeparator.

      The term "stack" is used conceptually and refers to indentArray[]. This
    array is used in adherence to the FIFO (first in, first out) feature of a
    stack.

  * Header Files and Definition Order

      All shared definitions (that do not exactly belong to one source code
    module and/or are shared by at least two source code modules) are stored
    in SharedDefs.h. Define statements only needed within a specific source
    code module are stored in the source code module's header file.
    Extern variable definitions are contained in their source code modules'
    header files.
    Function prototypes are either contained in the source code module in
    which they are implemented, or if they need to be public to other
    functions, they are contained in their source code modules' header files.

  * VRML Worlds and Camera & Light Definitions

      Vrml2OGL was written to allow the user to easily import 3D models into
    OpenGL. However, it was NOT written to convert whole VRML worlds.

      Since camera and light defintions are not truly part of an object but
    rather describe how the object is viewed, all commands pertaining to
    cameras and lights are ignored and will not appear in the output OpenGL
    file. In this way, the code prior to calling the generated object drawing
    function has total freedom to determine how the object is viewed.

  * Model Correction/Appearance
  
      In an attempt to fix some of the problems that I noticed during testing
    of Vrml2OGL, I added the -c, -cx, -cy, -cz, -rf and -rn options.

      Use any of the -cx, -cy or -cz options to manually adjust the center of
    an object. Otherwise, using the -c option, Vrml2OGL auto adjusts the
    center by calculating the middle of the object's bounding box and
    subtracting these coordinates from all vertex coordinates making up the
    object.
    Please note that the -c* options do not have an effect on coordinate
    transformations (see -it option) and vice-versa.

      If your model appears transparent or polygons faced opposite to the
    light source are lit, try the -rf and/or the -rn options.

      For more information on any of these options, please see their
    respective descriptions under the "I. Introduction" above.

      Since I was unable to properly view models with transparency or face
    ordering problems in a regular VRML viewer, I assume that these problems
    are contained in these VRML files and possibly stem from one or more
    conversions the models might have undergone, prior to arriving in VRML
    format.

      Note: Vrml2OGL auto-inserts a few lines of code (calls to glCullFace(),
    glBlend(), glEnable() and glDisable()) at the beginning and end of the
    each OpenGL file (*OGL.c). While this code generally improves the
    appearance of the model and its rendering performance, it can in certain
    situations cause exactly the opposite.
    GL_NORMALIZE will unnecessarily slow down rendering performance if neither
    glScale() nor glMultMatrix() are being used (either contained in the
    model's code through the -it option or applied by your own code prior to
    drawing the model).
    glCullFace() can make parts of a model disappear if the normals of those
    parts are set incorrectly.
    If you notice any of the negative effects just described, or know that for
    this model the above default settings are unnecessary/undesirable, you
    should manually comment out or erase the respective lines of code from the
    OpenGL file.

  * Major Shortcomings

      DEF tags are converted to comments instead of function names (provided
    the actual tag is located in between the DEF and the node specifier within
    _one_ line). Therefore, USE statements are ignored.
    Files actually referencing DEF tags via USE will not be converted
    properly. Depending on what kind of resources the USE statement refers
    to, Vrml2OGL might or might not issue an error message.

      Texture coordinates, indecis and bitmaps are not supported. Vrml2OGL
    will simply ignore any commands pertaining to textures.

      Most error messages issued by Vrml2OGL are specific to the kind of
    problem that occurred; however, unfortunately no line number is given
    indicating the position in the input file where the problem occurred.
    The -d option offers some help in this regard by outputting generic
    statements describing what Vrml2OGL is currently doing. However, only by
    carefully tracking down those statements and comparing them to the VRML
    code (in the input file) can an approximate fault position be located.
    I did not implement line numbers since I felt that debug statements were
    sufficient for my use. They might, however, be highly insufficient with
    large VRML files. I don't think that it would be too difficult to modifiy
    the program in order to add line numbers and might do so for a future
    version.

      Vrml2OGL expects the beginning and ending ("}") of node specifiers to be
    contained in separate lines. Once the handling function for a given node
    returns, after finding the end of the node it handled, the next node
    specifier or close brace is expected to be found in one of the following
    lines. Most files will adhere to this order; there might, however, be
    some files that do not adhere to this in which case, Vrml2OGL might
    produce incorrect results or terminate due to an incorrectly detected
    error (since the error most likely resulted from Vrml2OGL's inability to
    deal with node specifiers and node endings within one line).
    Examples of such problem lines would be "} Coordinate3" (Coordinate3 might
    not be registered causing the whole node to be ignored) or "}}" (the
    second close brace might not be registered causing a parent node not to be
    closed).
    This could be _fairly_ easily fixed by making each handling function blank
    out part of the line containing the end of the node up to and including
    the last character belonging to that node. In addition, the node specifier
    find function (findNodes()) would have to be adjusted so that it continues
    scanning the lines returned from handling functions for the next node
    specifier.

  * Data Precision

      Data read from the input VRML file is stored with its full precision in
    doubles. Any calculations necessary (such as the calculation of polygon
    normal vectors) are, therefore, performed at double precision.

      However, when output (to the 3 output files (OGL.c, Hdr.h and Dat.c))
    all data is rounded to a precision of 4 digits after the decimal point.

  * Status/Error Codes

      All status code defines end with the letters "ERR" indicating a certain
    error condition; an exception to this are "NOERR" and "SUCCESS". "NOERR"
    indicates that no error occurred, while "SUCCESS" not only indicates that
    no error occurred, but also that an operation succeeded.
    Most functions will return "NOERR" to indicate success; in some cases,
    however, where more than 3 general outcome stati (aborted due to an error,
    a request could not be fulfilled but is not considered an error, the
    request was fulfilled) can occurr, "SUCCESS" is returned (see
    readToBegOfNum() in Vrml2GLUtils.cpp.)

      Generic error code defines are preceded by the letters "GEN" and are
    indicated through a generic error message (see outputGenericErrorMsgs()
    in Vrml2GLUtils.cpp.)
    All error code defines NOT preceded by "GEN" are indicated by individual
    error messages (always written to stderr) following the source code that
    detected them.

      The following list shows all status and error codes used by Vrml2OGL.
    Also shown are the values associated with each code, and their
    descriptions. Depending on the outcome of the conversion, Vrml2OGL will
    return one of these values to the invocation environment.

    NOERR           0  no error was detected - the operation/conversion was
                       successful (see above)
    SUCCESS         1  internal use only - should not be returned to the
                       invocation environment (see above)
    OPENERR         2  open error - the indicated input or output file could
                       not be opened in the appropriate access mode
    SYNTAXERR       3  syntax error - this error code is used generically; it
                       is issued whenever Vrml2OGL cannot interpret part of
                       the input file as expected; it can only be understood
                       in the context of the displayed error message
    VERSIONERR      4  version error - the first line of the input file did
                       not contain the string "#VRML V1.0 ascii"
    NESTEDLODERR    5  nested LOD error - an LOD node was found within another
                       LOD
    OUTOFWARRAYERR  6  out of work array error - the size of the internal work
                       array (double workArray[MAXWORKARRAY]) was insufficient
                       in order to store a Coordinate3 or Normal array in it
    GENINPREADERR   7  generic input error - a read error was detected when
                       reading from the input file
    GENOUTPWRITEERR 8  generic output error - a write error was detected when
                       writing to the OGL.c file
    GENDATWRITEERR  9  generic Dat output error - a write error was detected
                       when writing to the Dat.c file
    GENHDRWRITEERR  10 generic Hdr output error - a write error was detected
                       when writing to the Hdr.h file
    GENALLOCERR     11 generic memory allocation error - a certain amount of
                       memory could not be allocated
    GENUNEXPEOFERR  12 generic unexpected end of file error - the end of the
                       input file was found at an invalid point (relative to
                       its context)
    GENFTELLERR     13 generic ftell() error - the file position in the input
                       file could not be retrieved
    GENFSEEKERR     14 generic fseek() error - could not adjust the file
                       position for one of the file pointers used for reading
                       the input file

  * Suggested Speed Improvements

      Currently Vrml2OGL (V1.0) uses 3 file pointers once it finds the
    beginning of an IndexedFaceSet node. It does this in order to read and
    output the material, normal and coordinate data for each vertex
    simultenously. All three file pointers start out at the beginning of the
    IndexedFaceSet node and read through the node, each looking for either the
    coordIndex, normalIndex or materialIndex specifiers prior to starting to
    read any of the indecis. Most of the time, the materialIndex specifier
    appears last, in which case, the file pointer that is looking for this
    node also has to read through all the indecis belonging to the normalIndex
    and coordIndex specifiers. Similarly, most of the time, the file pointer
    used for retrieving the normalIndex array has to read through the entire
    coordIndex array. This redundant reading of lines just read by another
    one of the 3 file pointers, costs a fair amount of time (particularly if
    the input file is a few MB big) and could be eliminated.

      The file could be read in larger chunks to minimize file access
    (instead of line by line.) The iostream C++ class might also provide a
    speed improvement over stdio functions such as fread() and fprintf().

      Vrml2OGL currently tries to read as little data into memory as possible
    (only Coordinate3, Normal and Material data). Though considering the
    memory capacity of most PCs nowadays, more data could be read into memory
    which could (if done correctly,) in turn, increase processing time.

  * Recognized Nodes

      The following nodes are recognized by Vrml2OGL:

    Separator           (see note below)
    Group               (see note below)
    Switch              (see note below)
    TransformSeparator  (see note below)
    LOD                 (see note below)
    Coordinate3
    ShapeHints          (see note below)
    IndexedFaceSet
    IndexedLineSet
    Normal
    Material
    Transform           (see note below)
    Translation         (see note below)
    Rotation            (see note below)
    Scale               (see note below)
    MatrixTransform     (see note below)
    Info                (see note below)

  * Unsupported Nodes

      The following nodes are ignored:

    NormalBinding       (see note below)
    MaterialBinding     (see note below)
    Cone
    Cube
    Cylinder
    Sphere
    PointSet
    Texture2            (see note above under "Major Shortcomings")
    TextureCoordinate2  (see note above under "Major Shortcomings")
    Texture2Transform   (see note above under "Major Shortcomings")
    DirectionalLight
    SpotLight
    PointLight
    OrhtographicCamera
    PerspectiveCamera
    FontStyle
    AsciiText
    WWWInline
    WWWAnchor

  * Various Notes

      The following notes give insight into how Vrml2OGL interprets .WRL files
    and indicate some of its shortcomings.

    * Separator
      - The renderCulling statement is ignored.
      - All attributes and the current transfrom matrix are pushed onto the
        stack when a separator node is found. These states are popped off the
        stack when the end of the node is found.

    * LOD
      - LODs are essentially ignored. Only the data defining the first
        (/highest) level of detail is retrieved while the data defining all
        remaining levels of detail is ignored.
        Each Separator that is a direct child to an LOD node is considered
        the beginning of a new level of detail. Therefore, the first Separator
        after the LOD node is considered the beginning of the first level of
        detail. Once this Separator's close brace is found, the next Separator
        is considered the beginning of the next level of detail and so on,
        until the close brace of the parent LOD node is found.
        Even when pre-scanning the input file, in order to find the bounding
        box (see findBoundingBox(),) only the first (/highest) level of detail
        of every LOD is included besides all coordinates outside of LOD nodes.

    * Switch
      - The Switch node can contain any type of child nodes. However, Vrml2OGL
        does not consider it as having any effect on its child nodes and
        therefore, only recognizes it in order to keep track of its open and
        close braces. Unlike the Separator node it does not cause a new stack
        entry or have any other effect.
      - The whichChild statment is ignored.

    * TransformSeparator
      - The TransformSeparator node can contain any type of child nodes. It is
        important to note that Vrml2OGL ignores its CTM saving effect and only
        keeps track of its open and close braces. Unlike the Separator node,
        it does not cause a new stack entry or have any other effect.
        This node is used seldomly.

    * Group
      - The Group node can contain any type of child nodes. However, Vrml2OGL
        does not consider it as having any effect on its child nodes and
        therefore, only recognizes it in order to keep track of its open and
        close braces. Unlike the Separator node, it does not cause a new stack
        entry or have any other effect.

    * ShapeHints
      - The vertexOrdering statement is recognized by Vrml2OGL and is output
        as glFrontFace(). If no vertexOrdering statement was found, the
        vertecis are assumed to be in counter clockwise order.
        Vrml2OGL generates default normals accordingly; 
        Note: The -rf option only affects the contents of the glFrontFace()
              statement. Default normals will still be calculated according to
              the vertex order retrieved from the input file (or according to
              the default vertex order, if the input file did not specify a
              vertexOrder for the vertecis being processed.)
              However, in order to reverse ALL normals (this includes
              default normals generated by Vrml2OGL as well as normals
              retrieved from the input file) use the -rn option.
      - All faces are assumed to be convex ,therefore, the faceType specifier
        is ignored.
      - The creaseAngle statement is ignored. It could be implemented by
        computing vertex normals (=average of all polygon normals of the
        polygons that contain this vertex (use vertex indecis in coordIndex
        array to find all polygons that contain this vertex)) for each vertex.
        Ignoring the creaseAngle statement causes some objects to look
        flat-shaded instead of smooth-shaded.

    * NormalBinding and MaterialBinding
      - Only PER_VERTEX_INDEXED and PER_FACE bindings are implemented and are
        figured depending on whether or not a current IndexedFacesSet node
        containing coordIndex data also contains normalIndex/materialIndex
        data. In absence of normalIndex/materialIndex data, NormalBinding/
        MaterialBinding (respectively) is assumed to be PER_FACE.

    * Transform
      - Only center, rotation, scaleFactor and translation statements have
        been implemented.
      - The scaleOrientation statement is ignored.
      - By default transformations are excluded from the OpenGL file, since
        in many cases, they position the model in an undesirable location in
        world space. Use the -it option in order to include all
        transformations.

    * Rotation, Scale and Translation
      - These nodes have been implemented and perform the same way as the
        respective commands for Transform nodes.
      - By default transformations are excluded from the OpenGL file, since
        in many cases, they position the model in an undesirable location in
        world space. Use the -it option in order to include all
        transformations.
  
    * # Comments
      - # comments are stripped off and dropped.

    * Info Nodes
      - These nodes are inserted into the OpenGL file as comments relative to
        the same location as in the VRML file.

VI. Revision Notes

  * Version 1.0 -> 1.01, August 1999 by Alexander Rohra

      All changes made are based upon suggestions and recommendations by
    Scott Nelson (nelson18@llnl.gov, http://www-dsed.llnl.gov,
    http://www-dsed.llnl.gov/documents/WWWtest.html).

    - Made a few minor modifications for UNIX compiler compatibility
    - Made the inclusion of the input file's base name in variable, define
      and function names dependent on whether or not NOMODELID was #defined
      upon compilation of Vrml2OGL.exe  

Copyright 1999 by Alexander Rohra. All rights reserved.
E n d  o f  D o c u m e n t
