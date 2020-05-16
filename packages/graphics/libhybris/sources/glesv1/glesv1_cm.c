/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
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
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

#include <hybris/common/binding.h>

#include "../egl/ws.h"

#define GLESV1_CM_LIBRARY_PATH "libGLESv1_CM.so"

HYBRIS_LIBRARY_INITIALIZE(glesv1_cm, GLESV1_CM_LIBRARY_PATH);

/* Scripts to generate these bindings can be found in utils/generate_glesv1/ */
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glAlphaFunc, GLenum, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glClearColor, GLclampf, GLclampf, GLclampf, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClearDepthf, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanef, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColor4f, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDepthRangef, GLclampf, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogf, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogfv, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glFrustumf, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetClipPlanef, GLenum, GLfloat *); /* was: GLfloat[4] */
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetFloatv, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetLightfv, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetMaterialfv, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexEnvfv, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexParameterfv, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelf, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelfv, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightf, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightfv, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLineWidth, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLoadMatrixf, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialf, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialfv, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glMultMatrixf, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glMultiTexCoord4f, GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glNormal3f, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glOrthof, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterf, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterfv, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glPointSize, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPolygonOffset, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glRotatef, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glScalef, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvf, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvfv, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterf, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterfv, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTranslatef, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glActiveTexture, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glAlphaFuncx, GLenum, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBindBuffer, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBindTexture, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBlendFunc, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glBufferData, GLenum, GLsizeiptr, const GLvoid *, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glBufferSubData, GLenum, GLintptr, GLsizeiptr, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClear, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glClearColorx, GLclampx, GLclampx, GLclampx, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClearDepthx, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClearStencil, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClientActiveTexture, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanex, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColor4ub, GLubyte, GLubyte, GLubyte, GLubyte);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColor4x, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColorMask, GLboolean, GLboolean, GLboolean, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColorPointer, GLint, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv1_cm, glCompressedTexImage2D, GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv1_cm, glCompressedTexSubImage2D, GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv1_cm, glCopyTexImage2D, GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION8(glesv1_cm, glCopyTexSubImage2D, GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glCullFace, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteBuffers, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteTextures, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDepthFunc, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDepthMask, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDepthRangex, GLclampx, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDisable, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDisableClientState, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glDrawArrays, GLenum, GLint, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glDrawElements, GLenum, GLsizei, GLenum, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glEnable, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glEnableClientState, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glFinish);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glFlush);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogx, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogxv, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glFrontFace, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glFrustumx, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetBooleanv, GLenum, GLboolean *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetBufferParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetClipPlanex, GLenum, GLfixed *); /* was: GLfixed[4] */
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenBuffers, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenTextures, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION0(glesv1_cm, GLenum, glGetError);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetFixedv, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetIntegerv, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetLightxv, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetMaterialxv, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetPointerv, GLenum, GLvoid **);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, const GLubyte *, glGetString, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexEnviv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexEnvxv, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexParameteriv, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexParameterxv, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glHint, GLenum, GLenum);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsBuffer, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsEnabled, GLenum);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsTexture, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelx, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelxv, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightx, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightxv, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLineWidthx, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glLoadIdentity);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLoadMatrixx, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLogicOp, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialx, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialxv, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glMatrixMode, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glMultMatrixx, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glMultiTexCoord4x, GLenum, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glNormal3x, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glNormalPointer, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glOrthox, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPixelStorei, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterx, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterxv, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glPointSizex, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPolygonOffsetx, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glPopMatrix);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glPushMatrix);
HYBRIS_IMPLEMENT_VOID_FUNCTION7(glesv1_cm, glReadPixels, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glRotatex, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glSampleCoverage, GLclampf, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glSampleCoveragex, GLclampx, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glScalex, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glScissor, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glShadeModel, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glStencilFunc, GLenum, GLint, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glStencilMask, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glStencilOp, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glTexCoordPointer, GLint, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvi, GLenum, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvx, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnviv, GLenum, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvxv, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv1_cm, glTexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameteri, GLenum, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterx, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameteriv, GLenum, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterxv, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION9(glesv1_cm, glTexSubImage2D, GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTranslatex, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glVertexPointer, GLint, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glViewport, GLint, GLint, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glPointSizePointerOES, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBlendEquationSeparateOES, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glBlendFuncSeparateOES, GLenum, GLenum, GLenum, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glBlendEquationOES, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glDrawTexsOES, GLshort, GLshort, GLshort, GLshort, GLshort);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glDrawTexiOES, GLint, GLint, GLint, GLint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glDrawTexxOES, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDrawTexsvOES, const GLshort *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDrawTexivOES, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDrawTexxvOES, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glDrawTexfOES, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDrawTexfvOES, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glEGLImageTargetRenderbufferStorageOES, GLenum, GLeglImageOES);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glAlphaFuncxOES, GLenum, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glClearColorxOES, GLclampx, GLclampx, GLclampx, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClearDepthxOES, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanexOES, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glColor4xOES, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDepthRangexOES, GLclampx, GLclampx);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogxOES, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glFogxvOES, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glFrustumxOES, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetClipPlanexOES, GLenum, GLfixed *); /* was: GLfixed[4] */
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetFixedvOES, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetLightxvOES, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetMaterialxvOES, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexEnvxvOES, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexParameterxvOES, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelxOES, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glLightModelxvOES, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightxOES, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glLightxvOES, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLineWidthxOES, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glLoadMatrixxOES, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialxOES, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glMaterialxvOES, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glMultMatrixxOES, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glMultiTexCoord4xOES, GLenum, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glNormal3xOES, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glOrthoxOES, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterxOES, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPointParameterxvOES, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glPointSizexOES, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glPolygonOffsetxOES, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glRotatexOES, GLfixed, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glSampleCoveragexOES, GLclampx, GLboolean);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glScalexOES, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvxOES, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexEnvxvOES, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterxOES, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexParameterxvOES, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTranslatexOES, GLfixed, GLfixed, GLfixed);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsRenderbufferOES, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBindRenderbufferOES, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteRenderbuffersOES, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenRenderbuffersOES, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glRenderbufferStorageOES, GLenum, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetRenderbufferParameterivOES, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsFramebufferOES, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glBindFramebufferOES, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteFramebuffersOES, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenFramebuffersOES, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLenum, glCheckFramebufferStatusOES, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glFramebufferRenderbufferOES, GLenum, GLenum, GLenum, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glFramebufferTexture2DOES, GLenum, GLenum, GLenum, GLuint, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glGetFramebufferAttachmentParameterivOES, GLenum, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glGenerateMipmapOES, GLenum);
HYBRIS_IMPLEMENT_FUNCTION2(glesv1_cm, void *, glMapBufferOES, GLenum, GLenum);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glUnmapBufferOES, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetBufferPointervOES, GLenum, GLenum, GLvoid **);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glCurrentPaletteMatrixOES, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glLoadPaletteFromModelViewMatrixOES);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glMatrixIndexPointerOES, GLint, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glWeightPointerOES, GLint, GLenum, GLsizei, const GLvoid *);
HYBRIS_IMPLEMENT_FUNCTION2(glesv1_cm, GLbitfield, glQueryMatrixxOES, GLfixed *, GLint *); /* was: GLfixed[16], GLint[16] */
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDepthRangefOES, GLclampf, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glFrustumfOES, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glOrthofOES, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanefOES, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGetClipPlanefOES, GLenum, GLfloat *); /* was: GLfloat[4] */
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glClearDepthfOES, GLclampf);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGenfOES, GLenum, GLenum, GLfloat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGenfvOES, GLenum, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGeniOES, GLenum, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGenivOES, GLenum, GLenum, const GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGenxOES, GLenum, GLenum, GLfixed);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glTexGenxvOES, GLenum, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexGenfvOES, GLenum, GLenum, GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexGenivOES, GLenum, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetTexGenxvOES, GLenum, GLenum, GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glBindVertexArrayOES, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteVertexArraysOES, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenVertexArraysOES, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsVertexArrayOES, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glRenderbufferStorageMultisampleAPPLE, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(glesv1_cm, glResolveMultisampleFramebufferAPPLE);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glDiscardFramebufferEXT, GLenum, GLsizei, const GLenum *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glMultiDrawArraysEXT, GLenum, const GLint *, const GLsizei *, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glMultiDrawElementsEXT, GLenum, const GLsizei *, GLenum, const void *const*, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanefIMG, GLenum, const GLfloat *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glClipPlanexIMG, GLenum, const GLfixed *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glRenderbufferStorageMultisampleIMG, GLenum, GLsizei, GLenum, GLsizei, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(glesv1_cm, glFramebufferTexture2DMultisampleIMG, GLenum, GLenum, GLenum, GLuint, GLint, GLsizei);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glDeleteFencesNV, GLsizei, const GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glGenFencesNV, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glIsFenceNV, GLuint);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glTestFenceNV, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetFenceivNV, GLuint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glFinishFenceNV, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glSetFenceNV, GLuint, GLenum);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glGetDriverControlsQCOM, GLint *, GLsizei, GLuint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glGetDriverControlStringQCOM, GLuint, GLsizei, GLsizei *, GLchar *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glEnableDriverControlQCOM, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glDisableDriverControlQCOM, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetTexturesQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetBuffersQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetRenderbuffersQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetFramebuffersQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glExtGetTexLevelParameterivQCOM, GLuint, GLenum, GLint, GLenum, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtTexObjectStateOverrideiQCOM, GLenum, GLenum, GLint);
HYBRIS_IMPLEMENT_VOID_FUNCTION11(glesv1_cm, glExtGetTexSubImageQCOM, GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(glesv1_cm, glExtGetBufferPointervQCOM, GLenum, GLvoid **);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetShadersQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(glesv1_cm, glExtGetProgramsQCOM, GLuint *, GLint, GLint *);
HYBRIS_IMPLEMENT_FUNCTION1(glesv1_cm, GLboolean, glExtIsProgramBinaryQCOM, GLuint);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(glesv1_cm, glExtGetProgramBinarySourceQCOM, GLuint, GLenum, GLchar *, GLint *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(glesv1_cm, glStartTilingQCOM, GLuint, GLuint, GLuint, GLuint, GLbitfield);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(glesv1_cm, glEndTilingQCOM, GLbitfield);

void glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
    static void (*_glEGLImageTargetTexture2DOES)(GLenum, GLeglImageOES) FP_ATTRIB = NULL;
    HYBRIS_DLSYSM(glesv1_cm, &_glEGLImageTargetTexture2DOES, "glEGLImageTargetTexture2DOES");
    struct egl_image *img = image;
    _glEGLImageTargetTexture2DOES(target, img ? img->egl_image : NULL);
}
