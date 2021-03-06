#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <string>
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "error.h"

void Model::add(Model to_add,glm::vec3 posV,int textureNum){
    //printf("mesh texturenum: %i\n",textureNum);
    int numTextures = getNumTextures();
    //printf("num textures:%i\n",numTextures);
    this->pos.reserve(this->pos.size()+to_add.pos.size());
    this->texCoord.reserve(this->texCoord.size()+to_add.texCoord.size());
    if(this->pos.size()==0){
        
    }
    int size = this->pos.size();
    for(int i =0;i<to_add.pos.size();i++){
        this->pos.push_back(to_add.pos[i]+posV);
        
        glm::vec2 temp = to_add.texCoord[i];
        temp.y+=(float) textureNum;
        temp.y/=(float) numTextures;
        this->texCoord.push_back(temp);
    }
    
    for(int i =0;i<to_add.indices.size();i++){
        this->indices.push_back(to_add.indices[i]+size);
    }
    for(int i =0;i<to_add.normal.size();i++){
        this->normal.push_back(to_add.normal[i]);
    }

}
std::vector<RunTimeModel> initMesh(std::vector<Model> models){
    std::vector<RunTimeModel> runmodels;
    runmodels.reserve(models.size());

    //unsigned int numindicies;
    for(int i =0;i<models.size();i++){
        GLuint vertex_array_object;//becomes meshNum in runtime model
        glGenVertexArrays(1,&vertex_array_object);

        glBindVertexArray(vertex_array_object);
        glError=glGetError();

        GLuint vertex_array_buffer[numBuffers];
        glGenBuffers(numBuffers,vertex_array_buffer);

        //buffering position
        glBindBuffer(GL_ARRAY_BUFFER,vertex_array_buffer[POSITION]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].pos[0])*models[i].pos.size(),
            &models[i].pos[0],GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

        glError=glGetError();
        //buffering Texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER,vertex_array_buffer[TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].texCoord[0])*models[i].texCoord.size(),
            &models[i].texCoord[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

        //buffering indicies
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vertex_array_buffer[INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(models[i].indices[0])*models[i].indices.size(),
        &models[i].indices[0],GL_STATIC_DRAW);

        //buffering Normal
        
        glBindBuffer(GL_ARRAY_BUFFER,vertex_array_buffer[NORMAL]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].normal[0])*models[i].normal.size(),
            &models[i].normal[0],GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
        

        glError=glGetError();
        if(glError!=0){
            printf("error: %i\n\n\n",glError);
        }
        runmodels.push_back(RunTimeModel(vertex_array_object,i,models[i].indices.size(),vertex_array_buffer));
    }
    glError=glGetError();
    return runmodels;
    
     
}
void drawMesh(RunTimeModel model,glm::vec3 position){

    //std::cout<<model.printString();
    glBindVertexArray(model.meshNum);
    glError=glGetError();//gl invalid operation
    glm::mat4 trans = glm::mat4(1.0f);
    
    sendTranslate(glm::translate(trans,position));
    glError=glGetError();
    glDrawElements(GL_TRIANGLES,model.numIndicies,GL_UNSIGNED_INT,0);
//Error Line
    glError=glGetError();
}
void updateMesh(std::vector<Model> models,std::vector<RunTimeModel> &to_update){
    if(to_update.size()!=models.size()){
        std::cout<<"to_update.size()!=models.size()\n";
    }
    for(int i=0;i<to_update.size();i++){
        glBindVertexArray(to_update[i].meshNum);
        glBindBuffer(GL_ARRAY_BUFFER,to_update[i].vertex_array_buffers[POSITION]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].pos[0])*models[i].pos.size(),
            &models[i].pos[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

       

        glBindBuffer(GL_ARRAY_BUFFER,to_update[i].vertex_array_buffers[POSITION]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].pos[0])*models[i].pos.size(),
            &models[i].pos[0],GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

        glError=glGetError();

        

        //buffering Texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER,to_update[i].vertex_array_buffers[TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(models[i].texCoord[0])*models[i].texCoord.size(),
            &models[i].texCoord[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

        //buffering indicies
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,to_update[i].vertex_array_buffers[INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(models[i].indices[0])*models[i].indices.size(),
        &models[i].indices[0],GL_STATIC_DRAW);
        
        to_update[i].numIndicies=models[i].indices.size();
    }
}
void deleteMesh(std::vector<RunTimeModel> &models){
    glDeleteVertexArrays(models.size(),&models[0].meshNum);
}