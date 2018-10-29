/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/**
 * 2d texture material, used for 2d edit mode
 * @module app/scripts/fgx/shaders/mattex2d
 */

// ******************************************************************
// imports
// ******************************************************************

// absoulte imports
import THREE from 'n3d-threejs';

// app imports
import RoiPalette from '../loaders/roipalette';

const TEX2D_PLANE_X = 0;
const TEX2D_PLANE_Y = 1;
const TEX2D_PLANE_Z = 2;

/** @class MaterialTex2d to render texture in 2d mode */

export default class MaterialTex2d {

  /** Simple material constructor
   * @constructor
   */
  constructor() {
    this.m_uniforms = {
      texture: { type: 't', value: null },
      palTexture: { type: 't', value: null },
      // plane values are: 0-x, 1-y, 2-z
      plane: { type: 'i', value: 0 },
      tilesHor: { type: 'i', value: 0 },
      sliceIndex: { type: 'i', value: 0 },
      numSlices: { type: 'i', value: 0 },
      zDim: { type: 'i', value: 0 },
      isRoiVolume: { type: 'i', value: 0 },
      width: { type: 'f', value: 0 },
      height: { type: 'f', value: 0 },
      currentGradient: { type: 'f', value: 0 }
    };
    this.m_strShaderVertex = `
      varying vec3 vecPos;
      varying vec2 vecUV;
      void main() {
        vecPos = position;
        vecUV = uv;
        gl_Position = vec4(position, 1.0);
      }
    `;
    this.m_strShaderFragment = ` /*переделать под один пиксель*/
      
      #define M_PI 3.1415926535897932384626433832795
      varying vec3 vecPos;
      varying vec2 vecUV;
      
      uniform sampler2D texture;
      uniform sampler2D palTexture;
      uniform int plane;
      uniform int tilesHor;
      uniform int sliceIndex;
      uniform int numSlices;
      uniform int zDim;
      uniform int isRoiVolume;
      uniform float width;
      uniform float height;
      uniform float currentGradient;
      
      const int SLICE_AXIS_X = 0;
      const int SLICE_AXIS_Y = 1;
      const int SLICE_AXIS_Z = 2;
      
      vec2 getTexCoord(vec3 vecVol) {
        float tileScale = 1.0 / float(tilesHor);
        vec2 vTex = vecVol.xy;
        vTex *= tileScale;
        int zSliceIndex = int(floor(vecVol.z * float(zDim)));
        
        // add tile x corner
        vTex.x += mod(float(zSliceIndex), float(tilesHor)) * tileScale;
        // add tile y corner
        vTex.y += floor(float(zSliceIndex) / float(tilesHor)) * tileScale;
        return vTex;
      }
      
      void main() {
        vec3 vVol = vec3(0.0, 0.0, 0.0);
        vec2 texCoordNear[9];
        float gradientsNear[9];
        vec2 vecUVtmp = vecUV;
        bool drawGradient = false;
        float angleValue;
        //getting texture coordinates of pixel and 8 pixels near him
        for(int i = 0;i<3;i++){
         for(int j = 0;j<3;j++){
           if(i<1)
             vecUVtmp.x -= (1.0/width);
           if(i>1)
             vecUVtmp.x += (1.0/width);
           if(j<1)
             vecUVtmp.y -= (1.0/height);
           if(j>1)
             vecUVtmp.y += (1.0/height);
           if (plane == SLICE_AXIS_Z) {
             vVol.x = vecUVtmp.x;
             vVol.y = vecUVtmp.y;
             vVol.z = float(sliceIndex) / float(numSlices);
           } // if z slices
           if (plane == SLICE_AXIS_Y) {
             vVol.x = vecUVtmp.x;
             vVol.y = float(sliceIndex) / float(numSlices);
             vVol.z = vecUVtmp.y;
          } // if y slices
          if (plane == SLICE_AXIS_X) {
             vVol.x = float(sliceIndex) / float(numSlices);
             vVol.y = vecUVtmp.x;
             vVol.z = vecUVtmp.y;
          } // if x slices
           texCoordNear[(i*3)+j] = getTexCoord(vVol);
         }
        }     
        // get texture pixel from source texture
        vec4 vColTex = texture2D(texture, texCoordNear[4]);
        vec4 vColTexNear[9];
        //getting gradients of pixel and 8 pixels near him
        for(int k = 0;k<9;k++)
        {
          vec2 tmpTexCoord = texCoordNear[k];
          //getting color of pixel and 8 pixels near him
          for(int i = 0;i<3;i++){
           for(int j = 0;j<3;j++){
              if(i<1)
                tmpTexCoord.x -= (1.0/width);
              if(i>1)
                tmpTexCoord.x += (1.0/width);
              if(j<1)
                tmpTexCoord.y -= (1.0/height);
              if(j>1)
                tmpTexCoord.y += (1.0/height);
              
              vColTexNear[(i*3)+j] = texture2D(texture,tmpTexCoord);
              tmpTexCoord = texCoordNear[k];
            }
          }
          //getting gradient from pixel (one from first nine)
          float intensity_x = -(vColTexNear[0].w+2.0*vColTexNear[3].w+vColTexNear[6].w)
                              +(vColTexNear[2].w+2.0*vColTexNear[4].w+vColTexNear[8].w);  
          float intensity_y = -(vColTexNear[0].w+2.0*vColTexNear[1].w+vColTexNear[2].w)
                              +(vColTexNear[6].w+2.0*vColTexNear[7].w+vColTexNear[8].w); 
      
            angleValue = atan(intensity_y/intensity_x);
            angleValue = angleValue*(180.0/M_PI);
            if((angleValue > 0.0 && angleValue < 22.5) || (angleValue > 157.5 && angleValue < 180.0))
              angleValue = 0.25;
                                           
          gradient = sqrt(intensity_x*intensity_x+intensity_y*intensity_y);
        }
        
         //non maximum suppression
          if(angleValue == 0.25){
            if(gradientsNear[1] < gradientsNear[4] && gradientsNear[4] < gradientsNear[7])
              drawGradient = true;
          }
          if(angleValue == 0.5){
            if(gradientsNear[2] < gradientsNear[4] && gradientsNear[4] < gradientsNear[6])
              drawGradient = true;
          }
          if(angleValue == 0.75){
            if(gradientsNear[3] < gradientsNear[4] && gradientsNear[4] < gradientsNear[5])
              drawGradient = true;
          }
          if(angleValue == 1.0){
            if(gradientsNear[0] < gradientsNear[4] && gradientsNear[4] < gradientsNear[8])
              drawGradient = true;
          }
          
        if(drawGradient == true && gradientsNear[4] > currentGradient){
            gl_FragColor = vec4(1.0,0.0,angleValue,1.0);
        }
        else if (isRoiVolume > 0) {
          float fPalIndex = vColTex.w;
          vec2 vPalCoord = vec2(fPalIndex, 0.0);
          vColTex = texture2D(palTexture, vPalCoord);
          gl_FragColor = vec4(vColTex.x, vColTex.y, vColTex.z, 1.0);
        } else {
          // use vec4.w component as a color. This is grey value, others (x,y,z) are used as normals, etc...
          float intensity = vColTex.w;
          gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
        }
      } // end of main
    `;
  }

  /** Create material for 2d texture display
   * @param {object} tex - texture with 2d layout of 3d source
   * @param {int} xDim - 3d texture dimension on x
   * @param {int} yDim - 3d texture dimension on y
   * @param {int} zDim - 3d texture dimension on z
   * @param {int} planeMode - see TEX2D_PLANE_X, TEX2D_PLANE_Y, TEX2D_PLANE_Z
   * @param {int} sliceIndex - current sliceIndex
   * @param {bool} isRoiVolume - is roi volume or not
   */
  create(tex, xDim, yDim, zDim, planeMode, sliceIndex, isRoiVolume) {
    this.m_uniforms.texture.value = tex;
    this.m_uniforms.plane.value = planeMode;
    this.m_uniforms.sliceIndex.value = sliceIndex;
    this.m_uniforms.zDim.value = zDim;
    this.m_uniforms.isRoiVolume.value = (isRoiVolume) ? 1 : 0;
    this.m_uniforms.tilesHor.value = Math.ceil(Math.sqrt(zDim));

    if (isRoiVolume) {
      console.log('MaterialTex2d: 2d texture will be rendered as roi indices');
      const roiPalette = new RoiPalette();
      const palette = roiPalette.getPalette256();
      const BYTES_PER_COLOR = 4;
      const MAGIC_COLOR = 250;
      const OFFS_0 = 0;
      const OFFS_1 = 1;
      const OFFS_2 = 2;

      const palB = palette[MAGIC_COLOR * BYTES_PER_COLOR + OFFS_0];
      const palG = palette[MAGIC_COLOR * BYTES_PER_COLOR + OFFS_1];
      const palR = palette[MAGIC_COLOR * BYTES_PER_COLOR + OFFS_2];
      console.log(`RoiPalette: pal[250] = ${palR}, ${palG}, ${palB}`);

      const TEX_W = 256;
      const TEX_H = 1;
      if (this.m_palTexture) {
        this.m_palTexture.dispose();
      }
      this.m_palTexture = new THREE.DataTexture(palette, TEX_W, TEX_H, THREE.RGBAFormat);
      this.m_palTexture.needsUpdate = true;
      this.m_palTexture.wrapS = THREE.ClampToEdgeWrapping;
      this.m_palTexture.wrapT = THREE.ClampToEdgeWrapping;
      this.m_palTexture.magFilter = THREE.NearestFilter;
      this.m_palTexture.minFilter = THREE.NearestFilter;
      this.m_uniforms.palTexture.value = this.m_palTexture;
    } else {
      this.m_uniforms.palTexture.value = null;
    }

    if (planeMode === TEX2D_PLANE_X) {
      this.m_uniforms.numSlices.value = xDim;
    } else if (planeMode === TEX2D_PLANE_Y) {
      this.m_uniforms.numSlices.value = yDim;
    } else if (planeMode === TEX2D_PLANE_Z) {
      this.m_uniforms.numSlices.value = zDim;
    }

    this.m_material = new THREE.ShaderMaterial({
      uniforms: this.m_uniforms,
      vertexShader: this.m_strShaderVertex,
      fragmentShader: this.m_strShaderFragment
    });
    this.m_material.needsUpdate = true;
  }
} // end of class
