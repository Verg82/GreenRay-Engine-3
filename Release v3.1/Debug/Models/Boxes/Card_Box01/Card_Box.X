xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material PDX01_-_Default_Material__25Sub0 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_top.bmp";
 }
}

Material PDX01_-_Default_Material__26Sub1 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_base.bmp";
 }
}

Material PDX01_-_Default_Material__27Sub2 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_sid1.bmp";
 }
}

Material PDX01_-_Default_Material__28Sub3 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_sid2.bmp";
 }
}

Material PDX01_-_Default_Material__29Sub4 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_sid3.bmp";
 }
}

Material PDX01_-_Default_Material__30Sub5 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Box1_sid4.bmp";
 }
}

Frame Box01 {
 

 FrameTransformMatrix {
  0.330954,0.000000,0.000000,0.000000,0.000000,0.330954,0.000000,0.000000,0.000000,0.000000,0.330954,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh  {
  24;
  -8.858268;0.000000;-8.858268;,
  8.858268;0.000000;-8.858268;,
  -8.858268;0.000000;8.858268;,
  8.858268;0.000000;8.858268;,
  -8.858268;19.685040;-8.858268;,
  8.858268;19.685040;-8.858268;,
  -8.858268;19.685040;8.858268;,
  8.858268;19.685040;8.858268;,
  -8.858268;0.000000;-8.858268;,
  8.858268;19.685040;-8.858268;,
  8.858268;0.000000;-8.858268;,
  -8.858268;19.685040;-8.858268;,
  8.858268;0.000000;-8.858268;,
  8.858268;19.685040;8.858268;,
  8.858268;0.000000;8.858268;,
  8.858268;19.685040;-8.858268;,
  8.858268;0.000000;8.858268;,
  -8.858268;19.685040;8.858268;,
  -8.858268;0.000000;8.858268;,
  8.858268;19.685040;8.858268;,
  -8.858268;0.000000;8.858268;,
  -8.858268;19.685040;-8.858268;,
  -8.858268;0.000000;-8.858268;,
  -8.858268;19.685040;8.858268;;
  12;
  3;0,3,2;,
  3;3,0,1;,
  3;4,7,5;,
  3;7,4,6;,
  3;8,9,10;,
  3;9,8,11;,
  3;12,13,14;,
  3;13,12,15;,
  3;16,17,18;,
  3;17,16,19;,
  3;20,21,22;,
  3;21,20,23;;

  MeshNormals  {
   6;
   0.000000;-1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;0.000000;-1.000000;,
   1.000000;0.000000;0.000000;,
   0.000000;0.000000;1.000000;,
   -1.000000;0.000000;0.000000;;
   12;
   3;0,0,0;,
   3;0,0,0;,
   3;1,1,1;,
   3;1,1,1;,
   3;2,2,2;,
   3;2,2,2;,
   3;3,3,3;,
   3;3,3,3;,
   3;4,4,4;,
   3;4,4,4;,
   3;5,5,5;,
   3;5,5,5;;
  }

  MeshMaterialList  {
   6;
   12;
   1,
   1,
   0,
   0,
   4,
   4,
   3,
   3,
   2,
   2,
   5,
   5;
   { PDX01_-_Default_Material__25Sub0 }
   { PDX01_-_Default_Material__26Sub1 }
   { PDX01_-_Default_Material__27Sub2 }
   { PDX01_-_Default_Material__28Sub3 }
   { PDX01_-_Default_Material__29Sub4 }
   { PDX01_-_Default_Material__30Sub5 }
  }

  MeshTextureCoords  {
   24;
   0.999501;0.999500;,
   0.000500;0.999501;,
   0.999500;0.000499;,
   0.000500;0.000500;,
   0.000500;0.000499;,
   0.000499;0.999500;,
   0.999501;0.000500;,
   0.999500;0.999501;,
   0.000500;0.999501;,
   0.999500;0.000499;,
   0.999500;0.999501;,
   0.000500;0.000499;,
   0.000500;0.999501;,
   0.999500;0.000499;,
   0.999500;0.999501;,
   0.000500;0.000499;,
   0.000500;0.999501;,
   0.999500;0.000499;,
   0.999500;0.999501;,
   0.000500;0.000499;,
   0.000500;0.999501;,
   0.999500;0.000499;,
   0.999500;0.999501;,
   0.000500;0.000499;;
  }
 }
}