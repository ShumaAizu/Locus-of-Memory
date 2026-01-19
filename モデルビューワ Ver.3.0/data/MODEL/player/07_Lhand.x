xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 24;
 1.05462;-0.12575;-0.95452;,
 -1.05465;-0.12575;-0.95452;,
 -1.38062;0.78573;-0.68338;,
 1.38061;0.78573;-0.68338;,
 1.38061;0.78573;0.68337;,
 -1.38062;0.78573;0.68337;,
 -1.05465;-0.12575;0.95452;,
 1.05462;-0.12575;0.95452;,
 1.05462;-0.12575;0.95452;,
 -1.05465;-0.12575;0.95452;,
 -1.05465;-0.12575;-0.95452;,
 1.05462;-0.12575;-0.95452;,
 1.05462;-0.12575;-0.95452;,
 1.38061;0.78573;-0.68338;,
 1.38061;0.78573;0.68337;,
 -1.05465;-0.12575;-0.95452;,
 -1.38062;0.78573;0.68337;,
 -1.38062;0.78573;-0.68338;,
 1.64394;4.76229;0.68337;,
 1.38061;0.78573;-0.68338;,
 1.64394;4.76229;-0.68338;,
 -1.64393;4.76229;0.68337;,
 -1.64393;4.76229;-0.68338;,
 -1.38062;0.78573;-0.68338;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,12,13,14;,
 4;15,9,16,17;,
 4;18,14,19,20;,
 4;21,16,14,18;,
 4;22,23,16,21;,
 4;20,19,23,22;,
 4;20,22,21,18;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\skin.png";
   }
  }
 }
 MeshNormals {
  14;
  0.000000;0.285124;-0.958491;,
  0.000000;0.144065;-0.989568;,
  0.000000;0.144070;0.989568;,
  0.000000;0.285134;0.958488;,
  0.997815;-0.066076;-0.000000;,
  -0.997815;-0.066071;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  0.941591;-0.336759;-0.000000;,
  0.979102;-0.203370;-0.000000;,
  -0.941597;-0.336741;0.000000;,
  -0.979104;-0.203358;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;;
  10;
  4;0,0,1,1;,
  4;2,2,3,3;,
  4;7,7,7,7;,
  4;8,8,9,9;,
  4;10,10,11,11;,
  4;4,9,9,4;,
  4;6,2,2,6;,
  4;5,11,11,5;,
  4;12,1,1,12;,
  4;13,13,13,13;;
 }
 MeshTextureCoords {
  24;
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
