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
      xDim: { type: 'i', value: 0 },
      yDim: { type: 'i', value: 0 },
      zDim: { type: 'i', value: 0 },
      isRoiVolume: { type: 'i', value: 0 },
      contrast: { type: 'f', value: 1.0 },
      brightness: { type: 'f', value: 0.0 },
      sigma: { type: 'f', value: 0.85 },
      //sigmaB: { type: 'f', value: 0.01 },
      xPixelSize: { type: 'f', value: 0.0 },
      yPixelSize: { type: 'f', value: 0.0 },
      flag: { type: 'b', value: false },
      showAll: { type: 'b', value: false },
      zoom: { type: 'f', value: 1 },
      posX: { type: 'f', value: 0 },
      posY: { type: 'f', value: 0 },
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
    this.m_strShaderFragment = `
      precision highp float;
      precision highp int;

      varying vec3 vecPos;
      varying vec2 vecUV;

      uniform sampler2D texture;
      uniform sampler2D palTexture;
      uniform int plane;
      uniform int tilesHor;
      uniform int sliceIndex;
      uniform int numSlices;
      uniform int xDim;
      uniform int yDim;
      uniform int zDim;
      uniform int isRoiVolume;
      uniform float contrast;
      uniform float brightness;
      uniform float sigma;
      uniform float zoom;
      uniform float posX;
      uniform float posY;
      //uniform float sigmaB;
      uniform float xPixelSize;
      uniform float yPixelSize;
      uniform bool flag;
      uniform bool showAll;
          
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
        // add 0.5 correction
        int xSize = xDim * tilesHor;
        int ySize = yDim * tilesHor;
        vTex.x += 0.5 / float(xSize);
        vTex.y += 0.5 / float(ySize);
        return vTex;
      }
      
      
      void main() 
      {   
        // special debug mode: show all texture
        if (showAll) {
          vec4 vColTex = texture2D(texture, vecUV);
          float intensity = vColTex.w;
          gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
          return;
        }
        vec3 vVol = vec3(0.0, 0.0, 0.0);
        vec2 texCoordNear[9];
        float gradientsNear[9];
        vec2 vecUVtmp = vecUV;
        
        for(int i = 0;i<3;i++){
         for(int j = 0;j<3;j++){
           if(i<1)
             vecUVtmp.x -= xPixelSize;
           if(i>1)
             vecUVtmp.x += xPixelSize;
           if(j<1)
             vecUVtmp.y -= yPixelSize;
           if(j>1)
             vecUVtmp.y += yPixelSize;
           if (plane == SLICE_AXIS_Z) {
             vVol.x = vecUVtmp.x* float(zoom) + float(posX) / 2.0;
             vVol.y = vecUVtmp.y* float(zoom) - float(posY) / 2.0;
             vVol.z = float(sliceIndex) / float(numSlices);
           } // if z slices
           if (plane == SLICE_AXIS_Y) {
             vVol.x = vecUVtmp.x* float(zoom) + float(posX) / 2.0;
             vVol.y = float(sliceIndex) / float(numSlices);
             vVol.z = vecUVtmp.y* float(zoom) - float(posY) / 2.0;
          } // if y slices
          if (plane == SLICE_AXIS_X) {
             vVol.x = float(sliceIndex) / float(numSlices);
             vVol.y = vecUVtmp.x* float(zoom) + float(posX) / 2.0;
             vVol.z = vecUVtmp.y* float(zoom) - float(posY) / 2.0;
          } // if x slices
           texCoordNear[(i*3)+j] = getTexCoord(vVol);
         }
        }  
        vec2 texCoord = getTexCoord(vVol);
        // get texture pixel from source texture
        vec4 vColTex = texture2D(texture, texCoord);
        vec4 vColTexNear[9];
        //calculating gradient
        for(int k = 0;k<9;k++)
        {
          vec2 tmpTexCoord = texCoordNear[k];
          //getting color of pixel and 8 pixels near him
          for(int i = 0;i<3;i++){
           for(int j = 0;j<3;j++){
              if(i<1)
                tmpTexCoord.x -= xPixelSize;
              if(i>1)
                tmpTexCoord.x += xPixelSize;
              if(j<1)
                tmpTexCoord.y -= yPixelSize;
              if(j>1)
                tmpTexCoord.y += yPixelSize;
              
              vColTexNear[(i*3)+j] = texture2D(texture,tmpTexCoord);
              tmpTexCoord = texCoordNear[k];
            }
          }
          //getting gradient from pixel (one from first nine)
          float intensity_x = -(vColTexNear[0].w+2.0*vColTexNear[3].w+vColTexNear[6].w)
                              +(vColTexNear[2].w+2.0*vColTexNear[4].w+vColTexNear[8].w);  
          float intensity_y = -(vColTexNear[0].w+2.0*vColTexNear[1].w+vColTexNear[2].w)
                              +(vColTexNear[6].w+2.0*vColTexNear[7].w+vColTexNear[8].w); 
      
           /* angleValue = atan(intensity_y/intensity_x);
            angleValue = angleValue*(180.0/M_PI);
            if((angleValue > 0.0 && angleValue < 22.5) || (angleValue > 157.5 && angleValue < 180.0))
              angleValue = 0.25;*/
                                           
          gradientsNear[k] = sqrt(intensity_x*intensity_x+intensity_y*intensity_y);
        }
        if (plane == SLICE_AXIS_Z) {
          vVol.x = vecUV.x * float(zoom) + float(posX) / 2.0;
          vVol.y = vecUV.y * float(zoom) - float(posY) / 2.0;
          vVol.z = float(sliceIndex) / float(numSlices);
        } // if z slices
        if (plane == SLICE_AXIS_Y) {
          vVol.x = vecUV.x * float(zoom) + float(posX) / 2.0;
          vVol.y = float(sliceIndex) / float(numSlices);
          vVol.z = vecUV.y * float(zoom) - float(posY) / 2.0;
        } // if y slices
        if (plane == SLICE_AXIS_X) {
          vVol.x = float(sliceIndex) / float(numSlices);
          vVol.y = vecUV.x * float(zoom) + float(posX) / 2.0;
          vVol.z = vecUV.y * float(zoom) - float(posY) / 2.0;
        } // if x slices
        float sigma2 = sigma*sigma;
        //float sigmaB2 = sigmaB*sigmaB;
       
        float val = texture2D(texture, texCoord).w;
        float TRIFICOBR = 0.0;
        float norm_factorB = 0.0;
        //Trilateral Filtering 
        for (float i = -1.0; i < 1.5; i += 1.0)
          for (float j = -1.0; j < 1.5; j += 1.0)
            for(float k = -1.0; k < 1.5; k += 1.0)
            {
              vec3 vAdd = vec3(0.0,0.0,0.0);
              if (plane == SLICE_AXIS_Z) {
                vAdd.x = xPixelSize * i;
                vAdd.y = yPixelSize * j;
                vAdd.z = k/float(numSlices);
              } // if z slices
              if (plane == SLICE_AXIS_Y) {
                vAdd.x = xPixelSize * i;
                vAdd.y = k/float(numSlices);
                vAdd.z = yPixelSize * j;
              } // if y slices
              if (plane == SLICE_AXIS_X) {
                vAdd.x = k/float(numSlices);
                vAdd.y = xPixelSize * i;
                vAdd.z = yPixelSize * j;
              } // if x slices          
              vec2 curTexCoord = getTexCoord(vVol + vAdd);  
              float curVal = texture2D(texture,curTexCoord).w;
              
              float min=1000.0,max=-1000.0;
              for(int a = 0; a < 9;a++){
                 if(gradientsNear[a] > max)
                 max = gradientsNear[a];
                 if(gradientsNear[a] < min)
                 min = gradientsNear[a];
              }
              float sigmab = abs(max-min);
              float sigmab2 = sigmab*sigmab;
              
              float N = 0.0;
              if(abs(gradientsNear[4-int(k)]-gradientsNear[4]) < sigma)
              {
                 N=1.0;
              }
              float trilateral = N*gradientsNear[4-int(k)]*exp( -(i*i + j*j + k*k) / (2.0 * sigma2)
               -(abs(gradientsNear[4]*gradientsNear[4] - gradientsNear[4-int(k)]*gradientsNear[4-int(k)])) / (2.0 * sigmab2));
              
               
              TRIFICOBR += curVal * trilateral;
              norm_factorB += trilateral;
            }
        // intensity
        TRIFICOBR = TRIFICOBR / norm_factorB;
        
        
        if (isRoiVolume > 0) {
          //float fPalIndex = vColTex.w;
          //vec2 vPalCoord = vec2(fPalIndex, 0.0);
          //vColTex = texture2D(palTexture, vPalCoord);
          gl_FragColor = vec4(vColTex.x, vColTex.y, vColTex.z, 1.0);
        } 
        
        TRIFICOBR = contrast * (TRIFICOBR - 0.5) + 0.5 + brightness;
        gl_FragColor = vec4(TRIFICOBR,TRIFICOBR,TRIFICOBR, 1.0);
        
        /*else {
          // use vec4.w component as a color. This is grey value, others (x,y,z) are used as normals, etc...
          float intensity = vColTex.w;
          gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
        }*/
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
    this.m_uniforms.xDim.value = xDim;
    this.m_uniforms.yDim.value = yDim;
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
