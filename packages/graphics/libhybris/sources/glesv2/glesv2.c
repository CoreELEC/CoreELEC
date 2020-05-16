/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 * Copyright (C) 2017 Bhushan Shah <bshah@kde.org>
 * Copyright (C) 2020 Matti Lehtimäki <matti.lehtimaki@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>

#include <GLES2/gl2ext.h>

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

#include <hybris/common/binding.h>

#include "../egl/ws.h"

// Android always uses libGLESv2.so for both OpenGL ES 2.0 and OpenGL ES 3.x
HYBRIS_LIBRARY_INITIALIZE(glesv2, getenv("LIBGLESV2") ? getenv("LIBGLESV2") : "libGLESv2.so");

HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glActiveTexture, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glAttachShader, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glBindAttribLocation, GLuint, GLuint, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindBuffer, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindFramebuffer, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindRenderbuffer, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindTexture, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glBlendColor, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glBlendEquation, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBlendEquationSeparate, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBlendFunc, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glBlendFuncSeparate, GLenum, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glBufferData, GLenum, GLsizeiptr, const void *, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glBufferSubData, GLenum, GLintptr, GLsizeiptr, const void *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLenum, glCheckFramebufferStatus, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glClear, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glClearColor, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glClearDepthf, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glClearStencil, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glColorMask, GLboolean, GLboolean, GLboolean, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glCompileShader, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glCompressedTexImage2D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv2, glCompressedTexSubImage2D, GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glCopyTexImage2D, GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glCopyTexSubImage2D, GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_FUNCTION0(glesv2, GLuint, glCreateProgram);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLuint, glCreateShader, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glCullFace, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteBuffers, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteFramebuffers, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDeleteProgram, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteRenderbuffers, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDeleteShader, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteTextures, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDepthFunc, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDepthMask, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDepthRangef, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDetachShader, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDisable, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDisableVertexAttribArray, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glDrawArrays, GLenum, GLint, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glDrawElements, GLenum, GLsizei, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glEnable, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glEnableVertexAttribArray, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glFinish);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glFlush);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glFrontFace, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenBuffers, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glGenerateMipmap, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenFramebuffers, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenRenderbuffers, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenTextures, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glGetActiveAttrib, GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glGetActiveUniform, GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetAttachedShaders, GLuint, GLsizei, GLsizei *, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLint, glGetAttribLocation, GLuint, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGetBooleanv, GLenum, GLboolean *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetBufferParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_FUNCTION0(glesv2, GLenum, glGetError);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGetFloatv, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetFramebufferAttachmentParameteriv, GLenum, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGetIntegerv, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetProgramiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetProgramInfoLog, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetRenderbufferParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetShaderiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetShaderInfoLog, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetShaderPrecisionFormat, GLenum, GLenum, GLint *, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetShaderSource, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, const GLubyte *, glGetString, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetTexParameterfv, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetTexParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetUniformfv, GLuint, GLint, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetUniformiv, GLuint, GLint, GLint *);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLint, glGetUniformLocation, GLuint, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetVertexAttribfv, GLuint, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetVertexAttribiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetVertexAttribPointerv, GLuint, GLenum, void **);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glHint, GLenum, GLenum);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsBuffer, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsEnabled, GLenum);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsFramebuffer, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsProgram, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsRenderbuffer, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsShader, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsTexture, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glLineWidth, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glLinkProgram, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glPixelStorei, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glPolygonOffset, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glReadPixels, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glReleaseShaderCompiler);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glSampleCoverage, GLfloat, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glScissor, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glShaderBinary, GLsizei, const GLuint *, GLenum, const void *, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glShaderSource, GLuint, GLsizei, const GLchar *const *, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glStencilFunc, GLenum, GLint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glStencilFuncSeparate, GLenum, GLenum, GLint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glStencilMask, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glStencilMaskSeparate, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glStencilOp, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glStencilOpSeparate, GLenum, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv2, glTexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameterf, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameterfv, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameteri, GLenum, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameteriv, GLenum, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv2, glTexSubImage2D, GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glUniform1f, GLint, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform1fv, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glUniform1i, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform1iv, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2f, GLint, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2fv, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2i, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2iv, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniform3f, GLint, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform3fv, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniform3i, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform3iv, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glUniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform4fv, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glUniform4i, GLint, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform4iv, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glUseProgram, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glValidateProgram, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttrib1f, GLuint, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttrib1fv, GLuint, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glVertexAttrib2f, GLuint, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttrib2fv, GLuint, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glVertexAttrib3f, GLuint, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttrib3fv, GLuint, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glVertexAttrib4f, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttrib4fv, GLuint, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glVertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glViewport, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glReadBuffer, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glDrawRangeElements, GLenum, GLuint, GLuint, GLsizei, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION10(glesv2, glTexImage3D, GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION11(glesv2, glTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv2, glCopyTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv2, glCompressedTexImage3D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION11(glesv2, glCompressedTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenQueries, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteQueries, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsQuery, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBeginQuery, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glEndQuery, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetQueryiv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetQueryObjectuiv, GLuint, GLenum, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glUnmapBuffer, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetBufferPointerv, GLenum, GLenum, void **);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDrawBuffers, GLsizei, const GLenum *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix2x3fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix3x2fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix2x4fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix4x2fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix3x4fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniformMatrix4x3fv, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION10(glesv2, glBlitFramebuffer, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glRenderbufferStorageMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glFramebufferTextureLayer, GLenum, GLenum, GLuint, GLint, GLint);
HYBRIS_IMPLEMENT_FUNCTION4(glesv2, void *, glMapBufferRange, GLenum, GLintptr, GLsizeiptr, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glFlushMappedBufferRange, GLenum, GLintptr, GLsizeiptr);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glBindVertexArray, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteVertexArrays, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenVertexArrays, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsVertexArray, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetIntegeri_v, GLenum, GLuint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glBeginTransformFeedback, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glEndTransformFeedback);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glBindBufferRange, GLenum, GLuint, GLuint, GLintptr, GLsizeiptr);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glBindBufferBase, GLenum, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glTransformFeedbackVaryings, GLuint, GLsizei, const GLchar *const *, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glGetTransformFeedbackVarying, GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glVertexAttribIPointer, GLuint, GLint, GLenum, GLsizei, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetVertexAttribIiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetVertexAttribIuiv, GLuint, GLenum, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glVertexAttribI4i, GLuint, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glVertexAttribI4ui, GLuint, GLuint, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttribI4iv, GLuint, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttribI4uiv, GLuint, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetUniformuiv, GLuint, GLint, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLint, glGetFragDataLocation, GLuint, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glUniform1ui, GLint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2ui, GLint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glUniform3ui, GLint, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glUniform4ui, GLint, GLuint, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform1uiv, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform2uiv, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform3uiv, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniform4uiv, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glClearBufferiv, GLenum, GLint, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glClearBufferuiv, GLenum, GLint, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glClearBufferfv, GLenum, GLint, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glClearBufferfi, GLenum, GLint, GLfloat, GLint);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, const GLubyte *, glGetStringi, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glCopyBufferSubData, GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetUniformIndices, GLuint, GLsizei, const GLchar *const *, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetActiveUniformsiv, GLuint, GLsizei, const GLuint *, GLenum, GLint *);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLuint, glGetUniformBlockIndex, GLuint, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetActiveUniformBlockiv, GLuint, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetActiveUniformBlockName, GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUniformBlockBinding, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glDrawArraysInstanced, GLenum, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glDrawElementsInstanced, GLenum, GLsizei, GLenum, const void *, GLsizei);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLsync, glFenceSync, GLenum, GLbitfield);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsSync, GLsync);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDeleteSync, GLsync);
HYBRIS_IMPLEMENT_FUNCTION3(glesv2, GLenum, glClientWaitSync, GLsync, GLbitfield, GLuint64);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glWaitSync, GLsync, GLbitfield, GLuint64);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGetInteger64v, GLenum, GLint64 *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetSynciv, GLsync, GLenum, GLsizei, GLsizei *, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetInteger64i_v, GLenum, GLuint, GLint64 *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetBufferParameteri64v, GLenum, GLenum, GLint64 *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenSamplers, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteSamplers, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsSampler, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindSampler, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameteri, GLuint, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameteriv, GLuint, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameterf, GLuint, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameterfv, GLuint, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetSamplerParameteriv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetSamplerParameterfv, GLuint, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttribDivisor, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBindTransformFeedback, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteTransformFeedbacks, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenTransformFeedbacks, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsTransformFeedback, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glPauseTransformFeedback);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glResumeTransformFeedback);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetProgramBinary, GLuint, GLsizei, GLsizei *, GLenum *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramBinary, GLuint, GLenum, const void *, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glProgramParameteri, GLuint, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glInvalidateFramebuffer, GLenum, GLsizei, const GLenum *);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glInvalidateSubFramebuffer, GLenum, GLsizei, const GLenum *, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glTexStorage2D, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glTexStorage3D, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetInternalformativ, GLenum, GLenum, GLenum, GLsizei, GLint *);

static void         (*_glEGLImageTargetTexture2DOES) (GLenum target, GLeglImageOES image) = NULL;

void glEGLImageTargetTexture2DOES (GLenum target, GLeglImageOES image)
{
       HYBRIS_DLSYSM(glesv2, &_glEGLImageTargetTexture2DOES, "glEGLImageTargetTexture2DOES");
       struct egl_image *img = image;
       (*_glEGLImageTargetTexture2DOES)(target, img ? img->egl_image : NULL);
}

/* GLES 3.1 */

HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glDispatchCompute, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glDispatchComputeIndirect, GLintptr);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDrawArraysIndirect, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glDrawElementsIndirect, GLenum, GLenum, const void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glFramebufferParameteri, GLenum, GLenum, GLint );
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetFramebufferParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetProgramInterfaceiv, GLuint, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_FUNCTION3(glesv2, GLuint, glGetProgramResourceIndex, GLuint, GLenum, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glGetProgramResourceName, GLuint, GLenum, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glGetProgramResourceiv, GLuint, GLenum, GLuint, GLsizei, const GLenum *, GLsizei, GLsizei *, GLint *);
HYBRIS_IMPLEMENT_FUNCTION3(glesv2, GLint, glGetProgramResourceLocation, GLuint, GLenum, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glUseProgramStages, GLuint, GLbitfield, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glActiveShaderProgram, GLuint, GLuint);
HYBRIS_IMPLEMENT_FUNCTION3(glesv2, GLuint, glCreateShaderProgramv, GLenum, GLsizei, const GLchar *const*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glBindProgramPipeline, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDeleteProgramPipelines, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGenProgramPipelines, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv2, GLboolean, glIsProgramPipeline, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetProgramPipelineiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glProgramUniform1i, GLuint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2i, GLuint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniform3i, GLuint, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glProgramUniform4i, GLuint, GLint, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glProgramUniform1ui, GLuint, GLint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2ui, GLuint, GLint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniform3ui, GLuint, GLint, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glProgramUniform4ui, GLuint, GLint, GLuint, GLuint, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glProgramUniform1f, GLuint, GLint, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2f, GLuint, GLint, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniform3f, GLuint, GLint, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glProgramUniform4f, GLuint, GLint, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform1iv, GLuint, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2iv, GLuint, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform3iv, GLuint, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform4iv, GLuint, GLint, GLsizei, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform1uiv, GLuint, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2uiv, GLuint, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform3uiv, GLuint, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform4uiv, GLuint, GLint, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform1fv, GLuint, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform2fv, GLuint, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform3fv, GLuint, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glProgramUniform4fv, GLuint, GLint, GLsizei, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix2x3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix3x2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix2x4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix4x2fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix3x4fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glProgramUniformMatrix4x3fv, GLuint, GLint, GLsizei, GLboolean, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glValidateProgramPipeline, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetProgramPipelineInfoLog, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glBindImageTexture, GLuint, GLuint, GLint, GLboolean, GLint, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetBooleani_v, GLenum, GLuint, GLboolean *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glMemoryBarrier, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glMemoryBarrierByRegion, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glTexStorage2DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetMultisamplefv, GLenum, GLuint, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glSampleMaski, GLuint, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetTexLevelParameteriv, GLenum, GLint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetTexLevelParameterfv, GLenum, GLint, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glBindVertexBuffer, GLuint, GLuint, GLintptr, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glVertexAttribFormat, GLuint, GLint, GLenum, GLboolean, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glVertexAttribIFormat, GLuint, GLint, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexAttribBinding, GLuint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glVertexBindingDivisor, GLuint, GLuint);

/* GLES 3.2 */

HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glBlendBarrier);
HYBRIS_IMPLEMENT_VOID_FUNCTION15(glesv2, glCopyImageSubData, GLuint, GLenum, GLint, GLint, GLint, GLint, GLuint, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glDebugMessageControl, GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glDebugMessageInsert, GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDebugMessageCallback, GLDEBUGPROC, const void *);
HYBRIS_IMPLEMENT_FUNCTION8(glesv2, GLuint, glGetDebugMessageLog, GLuint, GLsizei, GLenum *, GLenum *, GLuint *, GLenum *, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glPushDebugGroup, GLenum, GLuint, GLsizei, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv2, glPopDebugGroup);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glObjectLabel, GLenum, GLuint, GLsizei, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glGetObjectLabel, GLenum, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glObjectPtrLabel, const void *, GLsizei, const GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetObjectPtrLabel, const void *, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glGetPointerv, GLenum, void **);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glEnablei, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glDisablei, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glBlendEquationi, GLuint, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glBlendEquationSeparatei, GLuint, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glBlendFunci, GLuint, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glBlendFuncSeparatei, GLuint, GLenum, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glColorMaski, GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
HYBRIS_IMPLEMENT_FUNCTION2(glesv2, GLboolean, glIsEnabledi, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glDrawElementsBaseVertex, GLenum, GLsizei, GLenum, const void *, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glDrawRangeElementsBaseVertex, GLenum, GLuint, GLuint, GLsizei, GLenum, const void *, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv2, glDrawElementsInstancedBaseVertex, GLenum, GLsizei, GLenum, const void *, GLsizei, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glFramebufferTexture, GLenum, GLenum, GLuint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glPrimitiveBoundingBox, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_FUNCTION0(glesv2, GLenum, glGetGraphicsResetStatus);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv2, glReadnPixels, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetnUniformfv, GLuint, GLint, GLsizei, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetnUniformiv, GLuint, GLint, GLsizei, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv2, glGetnUniformuiv, GLuint, GLint, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv2, glMinSampleShading, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv2, glPatchParameteri, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameterIiv, GLenum, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexParameterIuiv, GLenum, GLenum, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetTexParameterIiv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetTexParameterIuiv, GLenum, GLenum, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameterIiv, GLuint, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glSamplerParameterIuiv, GLuint, GLenum, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetSamplerParameterIiv, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glGetSamplerParameterIuiv, GLuint, GLenum, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv2, glTexBuffer, GLenum, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv2, glTexBufferRange, GLenum, GLenum, GLuint, GLintptr, GLsizeiptr);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv2, glTexStorage3DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei, GLboolean);
