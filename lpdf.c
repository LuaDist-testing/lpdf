/*
* lpdf.c
* pdf library for Lua 5.1 based on PDFlib
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 11 Jul 2007 01:53:39
* This code is hereby placed in the public domain.
*/

#include <errno.h>
#include <string.h>

#include "pdflib.h"

#include "lua.h"
#include "lauxlib.h"

#ifndef lua_boxpointer
#define lua_boxpointer(L,u) \
	(*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#endif

#define MYNAME		"pdf"
#define MYVERSION	MYNAME " library for " LUA_VERSION " / Jul 2007 / "\
			"using PDFlib " PDFLIB_VERSIONSTRING
#define MYTYPE		MYNAME " handle"

#define getstring	luaL_checkstring
#define getostring(L,i)	luaL_optstring(L,i,NULL)
#define getfloat	(float) luaL_checknumber
#define getofloat(L,i)	(float) luaL_optnumber(L,i,0)
#define getint		  (int) luaL_checknumber
#define getoint(L,i)	  (int) luaL_optnumber(L,i,0)
#define getlong		 (long) luaL_checknumber

static int report(lua_State *L, int rc, const char *filename)
{
 if (rc==-1)
 {
  lua_pushnil(L);
  if (filename==NULL) return 1;
  lua_pushstring(L,strerror(errno));
  lua_pushstring(L,filename);
  return 3;
 }
 else
 {
  lua_pushnumber(L,rc);
  return 1;
 }
}

static PDF *getpdf(lua_State *L, int i)
{
 return *((void**)luaL_checkudata(L,i,MYTYPE));
}

static void errorhandler(PDF *p, int t, const char *message)
{
 lua_State *L = PDF_get_opaque(p);
 PDF_delete(p);
 luaL_error(L,"(PDFlib) %s",message);
}

static int do0(lua_State *L, void (*f)(PDF *p))
{
 PDF *p = getpdf(L,1);
 f(p);
 return 0;
}

static int Lnew(lua_State *L)			/** new() */
{
 PDF *p = PDF_new2(errorhandler,NULL,NULL,NULL,L);
 if (p==NULL)
  lua_pushnil(L);
 else
 {
  lua_boxpointer(L,p);
  luaL_getmetatable(L,MYTYPE);
  lua_setmetatable(L,-2);
 }
 return 1;
}

static int Ldelete(lua_State *L)		/** delete(p) */
{
 PDF *p = getpdf(L,1);
 PDF_delete(p);
 lua_pushnil(L);
 lua_setmetatable(L,1);
 return 0;
}

static int Lopen_file(lua_State *L)		/** open_file(p) */
{
 PDF *p = getpdf(L,1);
 const char *filename = getostring(L,2);
 int rc = PDF_open_file(p,filename);
 return report(L,rc,filename);
}

static int Lget_buffer(lua_State *L)		/** get_buffer(p) */
{
 PDF *p = getpdf(L,1);
 long size;
 const char *b = PDF_get_buffer(p,&size);
 lua_pushlstring(L,b,size);
 return 1;
}

static int Lclose(lua_State *L)			/** close(p) */
{
 return do0(L,PDF_close);
}

static int Lbegin_page(lua_State *L)		/** begin_page() */
{
 PDF *p = getpdf(L,1);
 float width = getfloat(L,2);
 float height = getfloat(L,3);
 PDF_begin_page(p,width,height);
 return 0;
}

static int Lend_page(lua_State *L)		/** end_page(p) */
{
 return do0(L,PDF_end_page);
}

static int Lget_value(lua_State *L)		/** get_value(p,key,modifier) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 float modifier = getofloat(L,3);
 float value = PDF_get_value(p,key,modifier);
 lua_pushnumber(L,value);
 return 1;
}

static int Lset_value(lua_State *L)		/** set_value(p,key,value) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 float value = getfloat(L,3);
 PDF_set_value(p,key,value);
 return 0;
}

static int Lget_parameter(lua_State *L)		/** get_parameter(p,key,modifier) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 float modifier = getofloat(L,3);
 const char *value = PDF_get_parameter(p,key,modifier);
 lua_pushstring(L,value);
 return 1;
}

static int Lset_parameter(lua_State *L)		/** set_parameter(p,key,value) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 const char *value = getstring(L,3);
 PDF_set_parameter(p,key,value);
 return 0;
}

static int Lcreate_pvf(lua_State *L)		/** create_pvf(p,filename,data,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *filename = getstring(L,2);
 const char *data = getstring(L,3);
 const char *optlist = getostring(L,4);
 PDF_create_pvf(p,filename,0,data,lua_strlen(L,3),optlist);
 return 0;
}

static int Ldelete_pvf(lua_State *L)		/** delete_pvf(p,filename) */
{
 PDF *p = getpdf(L,1);
 const char *filename = getstring(L,2);
 PDF_delete_pvf(p,filename,0);
 return 0;
}

static int Lload_font(lua_State *L)		/** load_font(p,fontname,encoding,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *fontname = getstring(L,2);
 const char *encoding = getstring(L,3);
 const char *optlist = getostring(L,4);
 int rc = PDF_load_font(p,fontname,0,encoding,optlist);
 return report(L,rc,NULL);
}

static int Lsetfont(lua_State *L)		/** setfont(p,font,fontsize) */
{
 PDF *p = getpdf(L,1);
 int font = getint(L,2);
 float fontsize = getfloat(L,3);
 PDF_setfont(p,font,fontsize);
 return 0;
}

static int Lbegin_font(lua_State *L)		/** begin_font(p,fontname,a,b,c,d,e,f,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *fontname = getstring(L,2);
 float a = getfloat(L,3);
 float b = getfloat(L,4);
 float c = getfloat(L,5);
 float d = getfloat(L,6);
 float e = getfloat(L,7);
 float f = getfloat(L,8);
 const char *optlist = getostring(L,9);
 PDF_begin_font(p,fontname,0,a,b,c,d,e,f,optlist);
 return 0;
}

static int Lend_font(lua_State *L)		/** end_font(p) */
{
 return do0(L,PDF_end_font);
}

static int Lbegin_glyph(lua_State *L)		/** begin_glyph(p,name,wx,llx,lly,urx,ury) */
{
 PDF *p = getpdf(L,1);
 const char *glyphname = getstring(L,2);
 float wx = getfloat(L,3);
 float llx = getfloat(L,4);
 float lly = getfloat(L,5);
 float urx = getfloat(L,6);
 float ury = getfloat(L,7);
 PDF_begin_glyph(p,glyphname,wx,llx,lly,urx,ury);
 return 0;
}

static int Lend_glyph(lua_State *L)		/** end_glyph(p) */
{
 return do0(L,PDF_end_glyph);
}

static int Lencoding_set_char(lua_State *L)	/** encoding_set_char(p,enconding,slot,glyphname,uv) */
{
 PDF *p = getpdf(L,1);
 const char *enconding = getstring(L,2);
 int slot = getint(L,3);
 const char *glyphname = getstring(L,4);
 int uv = getint(L,5);
 PDF_encoding_set_char(p,enconding,slot,glyphname,uv);
 return 0;
}

static int Lset_text_pos(lua_State *L)		/** set_text_pos(p,x,y) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 PDF_set_text_pos(p,x,y);
 return 0;
}

static int Lshow(lua_State *L)			/** show(p,text) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 PDF_show2(p,text,lua_strlen(L,2));
 return 0;
}

static int Lshow_xy(lua_State *L)		/** show_xy(p,text,x,y) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 float x = getfloat(L,3);
 float y = getfloat(L,4);
 PDF_show_xy2(p,text,lua_strlen(L,2),x,y);
 return 0;
}

static int Lcontinue_text(lua_State *L)		/** continue_text(p,text) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 PDF_continue_text2(p,text,lua_strlen(L,2));
 return 0;
}

static int Lfit_textline(lua_State *L)		/** fit_textline(p,text,x,y,optlist);
*/
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 float x = getfloat(L,3);
 float y = getfloat(L,4);
 const char *optlist = getostring(L,5);
 PDF_fit_textline(p,text,lua_strlen(L,2),x,y,optlist);
 return 0;
}

static int Lshow_boxed(lua_State *L)		/** show_boxed(p,text,left,top,width,height,hmode,feature) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 float left = getfloat(L,3);
 float top = getfloat(L,4);
 float width = getfloat(L,5);
 float height = getfloat(L,6);
 const char *hmode = getstring(L,7);
 const char *feature = getstring(L,8);
 int rc = PDF_show_boxed(p,text,left,top,width,height,hmode,feature);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lstringwidth(lua_State *L)		/** stringwidth(p,text,font,size) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 int font = getint(L,3);
 float size = getfloat(L,4);
 float width = PDF_stringwidth2(p,text,lua_strlen(L,2),font,size);
 lua_pushnumber(L,width);
 return 1;
}

static int Lsetdash(lua_State *L)		/** setdash(p,b,w) */
{
 PDF *p = getpdf(L,1);
 float b = getfloat(L,2);
 float w = getfloat(L,3);
 PDF_setdash(p,b,w);
 return 0;
}

static int Lsetdashpattern(lua_State *L)	/** setdashpattern(p,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *optlist = getostring(L,2);
 PDF_setdashpattern(p,optlist);
 return 0;
}

static int Lsetflat(lua_State *L)		/** setflat(p,flatness) */
{
 PDF *p = getpdf(L,1);
 float flatness = getfloat(L,2);
 PDF_setflat(p,flatness);
 return 0;
}

static int Lsetlinejoin(lua_State *L)		/** setlinejoin(p,linejoin) */
{
 PDF *p = getpdf(L,1);
 int linejoin = getint(L,2);
 PDF_setlinejoin(p,linejoin);
 return 0;
}

static int Lsetlinecap(lua_State *L)		/** setlinecap(p,linecap) */
{
 PDF *p = getpdf(L,1);
 int linecap = getint(L,2);
 PDF_setlinecap(p,linecap);
 return 0;
}

static int Lsetmiterlimit(lua_State *L)		/** setmiterlimit(p,miter) */
{
 PDF *p = getpdf(L,1);
 float miter = getfloat(L,2);
 PDF_setmiterlimit(p,miter);
 return 0;
}

static int Lsetlinewidth(lua_State *L)		/** setlinewidth(p,width) */
{
 PDF *p = getpdf(L,1);
 float width = getfloat(L,2);
 PDF_setlinewidth(p,width);
 return 0;
}

static int Linitgraphics(lua_State *L)		/** initgraphics(p) */
{
 return do0(L,PDF_initgraphics);
}

static int Lsave(lua_State *L)			/** save(p) */
{
 return do0(L,PDF_save);
}

static int Lrestore(lua_State *L)		/** restore(p) */
{
 return do0(L,PDF_restore);
}

static int Ltranslate(lua_State *L)		/** translate(p,x,y) */
{
 PDF *p = getpdf(L,1);
 float tx = getfloat(L,2);
 float ty = getfloat(L,3);
 PDF_translate(p,tx,ty);
 return 0;
}

static int Lscale(lua_State *L)			/** scale(p,x,y) */
{
 PDF *p = getpdf(L,1);
 float sx = getfloat(L,2);
 float sy = getfloat(L,3);
 PDF_scale(p,sx,sy);
 return 0;
}

static int Lrotate(lua_State *L)		/** rotate(p,phi) */
{
 PDF *p = getpdf(L,1);
 float phi = getfloat(L,2);
 PDF_rotate(p,phi);
 return 0;
}

static int Lskew(lua_State *L)			/** skew(p,alpha,beta) */
{
 PDF *p = getpdf(L,1);
 float alpha = getfloat(L,2);
 float beta = getfloat(L,3);
 PDF_skew(p,alpha,beta);
 return 0;
}

static int Lconcat(lua_State *L)		/** concat(p,a,b,c,d,e,f) */
{
 PDF *p = getpdf(L,1);
 float a = getfloat(L,2);
 float b = getfloat(L,3);
 float c = getfloat(L,4);
 float d = getfloat(L,5);
 float e = getfloat(L,6);
 float f = getfloat(L,7);
 PDF_concat(p,a,b,c,d,e,f);
 return 0;
}

static int Lsetmatrix(lua_State *L)		/** setmatrix(p,a,b,c,d,e,f) */
{
 PDF *p = getpdf(L,1);
 float a = getfloat(L,2);
 float b = getfloat(L,3);
 float c = getfloat(L,4);
 float d = getfloat(L,5);
 float e = getfloat(L,6);
 float f = getfloat(L,7);
 PDF_setmatrix(p,a,b,c,d,e,f);
 return 0;
}

static int Lcreate_gstate(lua_State *L)		/** create_gstate(p,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *optlist = getostring(L,2);
 int rc = PDF_create_gstate(p,optlist);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lset_gstate(lua_State *L)		/** set_gstate(p,gstate) */
{
 PDF *p = getpdf(L,1);
 int gstate = getint(L,2);
 PDF_set_gstate(p,gstate);
 return 0;
}

static int Lmoveto(lua_State *L)		/** moveto(p,x,y) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 PDF_moveto(p,x,y);
 return 0;
}

static int Llineto(lua_State *L)		/** lineto(p,x,y) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 PDF_lineto(p,x,y);
 return 0;
}

static int Lcurveto(lua_State *L)		/** curveto(p,x1,y1,x2,y2,x3,y3) */
{
 PDF *p = getpdf(L,1);
 float x1 = getfloat(L,2);
 float y1 = getfloat(L,3);
 float x2 = getfloat(L,4);
 float y2 = getfloat(L,5);
 float x3 = getfloat(L,6);
 float y3 = getfloat(L,7);
 PDF_curveto(p,x1,y1,x2,y2,x3,y3);
 return 0;
}

static int Lcircle(lua_State *L)		/** circle(p,x,y,r) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 float r = getfloat(L,4);
 PDF_circle(p,x,y,r);
 return 0;
}

static int Larc(lua_State *L)			/** arc(p,x,y,r,alpha,beta) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 float r = getfloat(L,4);
 float alpha = getfloat(L,5);
 float beta = getfloat(L,6);
 PDF_arc(p,x,y,r,alpha,beta);
 return 0;
}

static int Larcn(lua_State *L)			/** arcn(p,x,y,r,alpha,beta) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 float r = getfloat(L,4);
 float alpha = getfloat(L,5);
 float beta = getfloat(L,6);
 PDF_arcn(p,x,y,r,alpha,beta);
 return 0;
}

static int Lrect(lua_State *L)			/** rect(p,x,y,width,height) */
{
 PDF *p = getpdf(L,1);
 float x = getfloat(L,2);
 float y = getfloat(L,3);
 float width = getfloat(L,4);
 float height = getfloat(L,5);
 PDF_rect(p,x,y,width,height);
 return 0;
}

static int Lclosepath(lua_State *L)		/** closepath(p) */
{
 return do0(L,PDF_closepath);
}

static int Lstroke(lua_State *L)		/** stroke(p) */
{
 return do0(L,PDF_stroke);
}

static int Lclosepath_stroke(lua_State *L)	/** closepath_stroke(p) */
{
 return do0(L,PDF_closepath_stroke);
}

static int Lfill(lua_State *L)			/** fill(p) */
{
 return do0(L,PDF_fill);
}

static int Lfill_stroke(lua_State *L)		/** fill_stroke(p) */
{
 return do0(L,PDF_fill_stroke);
}

static int Lclosepath_fill_stroke(lua_State *L)	/** closepath_fill_stroke(p) */
{
 return do0(L,PDF_closepath_fill_stroke);
}

static int Lclip(lua_State *L)			/** clip(p) */
{
 return do0(L,PDF_clip);
}

static int Lendpath(lua_State *L)		/** endpath(p) */
{
 return do0(L,PDF_endpath);
}

static int Lsetcolor(lua_State *L)		/** setcolor(p,fstype,colorspace,c1,c2,c3,c4) */
{
 PDF *p = getpdf(L,1);
 const char *fstype = getstring(L,2);
 const char *colorspace = getstring(L,3);
 float c1 = getofloat(L,4);
 float c2 = getofloat(L,5);
 float c3 = getofloat(L,6);
 float c4 = getofloat(L,7);
 PDF_setcolor(p,fstype,colorspace,c1,c2,c3,c4);
 return 0;
}

static int Lmakespotcolor(lua_State *L)		/** makespotcolor(p,spotname) */
{
 PDF *p = getpdf(L,1);
 const char *spotname = getstring(L,2);
 int rc = PDF_makespotcolor(p,spotname,0);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lload_iccprofile(lua_State *L)	/** load_iccprofile(p,profilename,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *profilename = getstring(L,2);
 const char *optlist = getostring(L,3);
 int rc = PDF_load_iccprofile(p,profilename,0,optlist);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lbegin_pattern(lua_State *L)		/** begin_pattern(p,width,height,xstep,ystep,painttype) */
{
 PDF *p = getpdf(L,1);
 float width = getfloat(L,2);
 float height = getfloat(L,3);
 float xstep = getfloat(L,4);
 float ystep = getfloat(L,5);
 int painttype = getint(L,6);
 int rc = PDF_begin_pattern(p,width,height,xstep,ystep,painttype);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lend_pattern(lua_State *L)		/** end_pattern(p) */
{
 return do0(L,PDF_end_pattern);
}

static int Lshading_pattern(lua_State *L)	/** shading_pattern(p,shading,optlist) */
{
 PDF *p = getpdf(L,1);
 int shading = getint(L,2);
 const char *optlist = getostring(L,3);
 int rc = PDF_shading_pattern(p,shading,optlist);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lshfill(lua_State *L)		/** shfill(p,shading) */
{
 PDF *p = getpdf(L,1);
 int shading = getint(L,2);
 PDF_shfill(p,shading);
 return 0;
}

static int Lshading(lua_State *L)		/** shading(p,shtype,x0,y0,x1,y1,c1,c2,c3,c4,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *shtype = getstring(L,2);
 float x0 = getfloat(L,3);
 float y0 = getfloat(L,4);
 float x1 = getfloat(L,5);
 float y1 = getfloat(L,6);
 float c1 = getfloat(L,7);
 float c2 = getfloat(L,8);
 float c3 = getfloat(L,9);
 float c4 = getfloat(L,10);
 const char *optlist = getostring(L,11);
 PDF_shading(p,shtype,x0,y0,x1,y1,c1,c2,c3,c4,optlist);
 return 0;
}

static int Lload_image(lua_State *L)		/** load_image(p,imagetype,filename,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *imagetype = getstring(L,2);
 const char *filename = getstring(L,3);
 const char *optlist = getostring(L,4);
 int rc = PDF_load_image(p,imagetype,filename,0,optlist);
 return report(L,rc,filename);
}

static int Lclose_image(lua_State *L)		/** close_image(p,image) */
{
 PDF *p = getpdf(L,1);
 int image = getint(L,2);
 PDF_close_image(p,image);
 return 0;
}

static int Lfit_image(lua_State *L)		/** fit_image(p,image,x,y,optlist) */
{
 PDF *p = getpdf(L,1);
 int image = getint(L,2);
 float x = getfloat(L,3);
 float y = getfloat(L,4);
 const char *optlist = getostring(L,5);
 PDF_fit_image(p,image,x,y,optlist);
 return 0;
}

static int Lbegin_template(lua_State *L)	/** begin_template(p,width,height) */
{
 PDF *p = getpdf(L,1);
 float width = getfloat(L,2);
 float height = getfloat(L,3);
 int rc = PDF_begin_template(p,width,height);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lend_template(lua_State *L)		/** end_template(p) */
{
 return do0(L,PDF_end_template);
}

static int Lopen_pdi(lua_State *L)		/** open_pdi(p,filename,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *filename = getstring(L,2);
 const char *optlist = getostring(L,3);
 int rc = PDF_open_pdi(p,filename,optlist,0);
 return report(L,rc,filename);
}

static int Lclose_pdi(lua_State *L)		/** close_pdi(p,doc) */
{
 PDF *p = getpdf(L,1);
 int doc = getint(L,2);
 PDF_close_pdi(p,doc);
 return 0;
}

static int Lopen_pdi_page(lua_State *L)		/** open_pdi_page(p,doc,page,optlist) */
{
 PDF *p = getpdf(L,1);
 int doc = getint(L,2);
 int page = getint(L,3);
 const char *optlist = getostring(L,4);
 int rc = PDF_open_pdi_page(p,doc,page,optlist);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lclose_pdi_page(lua_State *L)	/** close_pdi_page(p,page) */
{
 PDF *p = getpdf(L,1);
 int page = getint(L,2);
 PDF_close_pdi_page(p,page);
 return 0;
}

static int Lfit_pdi_page(lua_State *L)		/** fit_pdi_page(p,page,x,y,optlist) */
{
 PDF *p = getpdf(L,1);
 int page = getint(L,2);
 float x = getfloat(L,3);
 float y = getfloat(L,4);
 const char *optlist = getostring(L,5);
 PDF_fit_pdi_page(p,page,x,y,optlist);
 return 0;
}

static int Lprocess_pdi(lua_State *L)		/** process_pdi(p,doc,page,optlist) */
{
 PDF *p = getpdf(L,1);
 int doc = getint(L,2);
 int page = getint(L,3);
 const char *optlist = getostring(L,4);
 PDF_process_pdi(p,doc,page,optlist);
 return 0;
}

static int Lget_pdi_value(lua_State *L)		/** get_pdi_value(p,key,doc,page) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 int doc = getint(L,3);
 int page = getint(L,4);
 float rc = PDF_get_pdi_value(p,key,doc,page,0);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lget_pdi_parameter(lua_State *L)	/** get_pdi_parameter(p,key,doc,page) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 int doc = getint(L,3);
 int page = getint(L,4);
 int len;
 const char *s = PDF_get_pdi_parameter(p,key,doc,page,0,&len);
 lua_pushlstring(L,s,len);
 return 1;
}

static int Lfill_textblock(lua_State *L)	/** fill_textblock(p,page,blockname,text,optlist) */
{
 PDF *p = getpdf(L,1);
 int page = getint(L,2);
 const char *blockname = getstring(L,3);
 const char *text = getstring(L,4);
 const char *optlist = getostring(L,5);
 int rc = PDF_fill_textblock(p,page,blockname,text,lua_strlen(L,4),optlist);
 return report(L,rc,NULL);
}

static int Lfill_pdfblock(lua_State *L)	/** fill_pdfblock(p,page,blockname,pdf,optlist) */
{
 PDF *p = getpdf(L,1);
 int page = getint(L,2);
 const char *blockname = getstring(L,3);
 int contents = getint(L,4);
 const char *optlist = getostring(L,5);
 int rc = PDF_fill_pdfblock(p,page,blockname,contents,optlist);
 return report(L,rc,NULL);
}

static int Ladd_bookmark(lua_State *L)		/** add_bookmark(p,text,parent,open) */
{
 PDF *p = getpdf(L,1);
 const char *text = getstring(L,2);
 int parent = getint(L,3);
 int open = getint(L,4);
 int rc = PDF_add_bookmark2(p,text,lua_strlen(L,2),parent,open);
 lua_pushnumber(L,rc);
 return 1;
}

static int Lset_info(lua_State *L)		/** set_info(p,key,value) */
{
 PDF *p = getpdf(L,1);
 const char *key = getstring(L,2);
 const char *value = getstring(L,3);
 PDF_set_info2(p,key,value,lua_strlen(L,3));
 return 0;
}

static int Lattach_file(lua_State *L)		/** attach_file(p,llx,lly,urx,ury,filename,description,author,mimetype,icon) */

{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 const char *filename = getstring(L,6);
 const char *description = getstring(L,7);
 const char *author = getstring(L,8);
 const char *mimetype = getstring(L,9);
 const char *icon = getstring(L,10);
 PDF_attach_file2(p,llx,lly,urx,ury,filename,0,description,lua_strlen(L,7),author,lua_strlen(L,8),mimetype,icon);
 return 0;
}

static int Ladd_note(lua_State *L)		/** add_note(p,llx,lly,urx,ury,contents,title,icon,open) */
{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 const char *contents = getstring(L,6);
 const char *title = getstring(L,7);
 const char *icon = getstring(L,8);
 int open = getint(L,9);
 PDF_add_note2(p,llx,lly,urx,ury,contents,lua_strlen(L,6),title,lua_strlen(L,7),icon,open);
 return 0;
}

static int Ladd_pdflink(lua_State *L)		/** add_pdflink(p,llx,lly,urx,ury,filename,page,optlist) */
{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 const char *filename = getstring(L,6);
 int page = getint(L,7);
 const char *optlist = getostring(L,8);
 PDF_add_pdflink(p,llx,lly,urx,ury,filename,page,optlist);
 return 0;
}

static int Ladd_locallink(lua_State *L)		/** add_locallink(p,llx,lly,urx,ury,filename,page,optlist) */
{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 int page = getint(L,6);
 const char *optlist = getostring(L,7);
 PDF_add_locallink(p,llx,lly,urx,ury,page,optlist);
 return 0;
}

static int Ladd_launchlink(lua_State *L)	/** add_launchlink(p,llx,lly,urx,ury,filename) */
{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 const char *filename = getstring(L,6);
 PDF_add_launchlink(p,llx,lly,urx,ury,filename);
 return 0;
}

static int Ladd_weblink(lua_State *L)		/** add_weblink(p,llx,lly,urx,ury,url) */
{
 PDF *p = getpdf(L,1);
 float llx = getfloat(L,2);
 float lly = getfloat(L,3);
 float urx = getfloat(L,4);
 float ury = getfloat(L,5);
 const char *url = getstring(L,6);
 PDF_add_weblink(p,llx,lly,urx,ury,url);
 return 0;
}

static int Lset_border_style(lua_State *L)	/** set_border_style(p,style,width) */
{
 PDF *p = getpdf(L,1);
 const char *style = getstring(L,2);
 float width = getfloat(L,3);
 PDF_set_border_style(p,style,width);
 return 0;
}

static int Lset_border_color(lua_State *L)	/** set_border_color(p,red,green,blue) */
{
 PDF *p = getpdf(L,1);
 float red = getfloat(L,2);
 float green = getfloat(L,3);
 float blue = getfloat(L,4);
 PDF_set_border_color(p,red,green,blue);
 return 0;
}

static int Lset_border_dash(lua_State *L)	/** set_border_dash(p,b,w) */
{
 PDF *p = getpdf(L,1);
 float b = getfloat(L,2);
 float w = getfloat(L,3);
 PDF_set_border_dash(p,b,w);
 return 0;
}

static int Ladd_nameddest(lua_State *L)		/** add_nameddest(p,name,optlist) */
{
 PDF *p = getpdf(L,1);
 const char *name = getstring(L,2);
 const char *optlist = getostring(L,7);
 PDF_add_nameddest(p,name,0,optlist);
 return 0;
}

static int Ladd_thumbnail(lua_State *L)		/** add_thumbnail(p,image) */
{
 PDF *p = getpdf(L,1);
 int image = getint(L,2);
 PDF_add_thumbnail(p,image);
 return 0;
}

static int Ltostring(lua_State *L)		/** tostring(p) */
{
 PDF *p = getpdf(L,1);
 lua_pushfstring(L,"%s %p",MYTYPE,(void*)p);
 return 1;
}

static const luaL_reg R[] =
{
	{ "__gc",		Ldelete },
	{ "__tostring",		Ltostring },
	{ "add_bookmark",	Ladd_bookmark },
	{ "add_launchlink",	Ladd_launchlink },
	{ "add_locallink",	Ladd_locallink },
	{ "add_nameddest",	Ladd_nameddest },
	{ "add_note",		Ladd_note },
	{ "add_pdflink",	Ladd_pdflink },
	{ "add_thumbnail",	Ladd_thumbnail },
	{ "add_weblink",	Ladd_weblink },
	{ "arc",		Larc },
	{ "arcn",		Larcn },
	{ "attach_file",	Lattach_file },
	{ "begin_font",		Lbegin_font },
	{ "begin_glyph",	Lbegin_glyph },
	{ "begin_page",		Lbegin_page },
	{ "begin_pattern",	Lbegin_pattern },
	{ "begin_template",	Lbegin_template },
	{ "circle",		Lcircle },
	{ "clip",		Lclip },
	{ "close",		Lclose },
	{ "close_image",	Lclose_image },
	{ "close_pdi",		Lclose_pdi },
	{ "close_pdi_page",	Lclose_pdi_page },
	{ "closepath",		Lclosepath },
	{ "closepath_fill_stroke",	Lclosepath_fill_stroke },
	{ "closepath_stroke",	Lclosepath_stroke },
	{ "concat",		Lconcat },
	{ "continue_text",	Lcontinue_text },
	{ "create_gstate",	Lcreate_gstate },
	{ "create_pvf",		Lcreate_pvf },
	{ "curveto",		Lcurveto },
	{ "delete",		Ldelete },
	{ "delete_pvf",		Ldelete_pvf },
	{ "encoding_set_char",		Lencoding_set_char },
	{ "end_font",		Lend_font },
	{ "end_glyph",		Lend_glyph },
	{ "end_page",		Lend_page },
	{ "end_pattern",	Lend_pattern },
	{ "end_template",	Lend_template },
	{ "endpath",		Lendpath },
	{ "fill",		Lfill },
	{ "fill_pdfblock",	Lfill_pdfblock },
	{ "fill_stroke",	Lfill_stroke },
	{ "fill_textblock",	Lfill_textblock },
	{ "fit_image",		Lfit_image },
	{ "fit_pdi_page",	Lfit_pdi_page },
	{ "fit_textline",	Lfit_textline },
	{ "get_buffer",		Lget_buffer },
	{ "get_parameter",	Lget_parameter },
	{ "get_pdi_parameter",	Lget_pdi_parameter },
	{ "get_pdi_value",	Lget_pdi_value },
	{ "get_value",		Lget_value },
	{ "initgraphics",	Linitgraphics },
	{ "lineto",		Llineto },
	{ "load_font",		Lload_font },
	{ "load_iccprofile",	Lload_iccprofile },
	{ "load_image",		Lload_image },
	{ "makespotcolor",	Lmakespotcolor },
	{ "moveto",		Lmoveto },
	{ "new",		Lnew },
	{ "open_file",		Lopen_file },
	{ "open_pdi",		Lopen_pdi },
	{ "open_pdi_page",	Lopen_pdi_page },
	{ "process_pdi",	Lprocess_pdi },
	{ "rect",		Lrect },
	{ "restore",		Lrestore },
	{ "rotate",		Lrotate },
	{ "save",		Lsave },
	{ "scale",		Lscale },
	{ "set_border_color",	Lset_border_color },
	{ "set_border_dash",	Lset_border_dash },
	{ "set_border_style",	Lset_border_style },
	{ "set_gstate", 	Lset_gstate },
	{ "set_info",		Lset_info },
	{ "set_parameter",	Lset_parameter },
	{ "set_text_pos",	Lset_text_pos },
	{ "set_value",		Lset_value },
	{ "setcolor",		Lsetcolor },
	{ "setdash",		Lsetdash },
	{ "setdashpattern",	Lsetdashpattern },
	{ "setflat",		Lsetflat },
	{ "setfont",		Lsetfont },
	{ "setlinecap",		Lsetlinecap },
	{ "setlinejoin",	Lsetlinejoin },
	{ "setlinewidth",	Lsetlinewidth },
	{ "setmatrix",		Lsetmatrix },
	{ "setmiterlimit",	Lsetmiterlimit },
	{ "shading",    	Lshading },
	{ "shading_pattern",	Lshading_pattern },
	{ "shfill",     	Lshfill	 },
	{ "show",		Lshow },
	{ "show_boxed",		Lshow_boxed },
	{ "show_xy",		Lshow_xy },
	{ "skew",		Lskew },
	{ "stringwidth",	Lstringwidth },
	{ "stroke",		Lstroke },
	{ "tostring",		Ltostring },
	{ "translate",		Ltranslate },
	{ NULL,			NULL }
};

LUALIB_API int luaopen_pdf(lua_State *L)
{
 luaL_newmetatable(L,MYTYPE);
 lua_pushvalue(L,-1);
 luaL_openlib(L,NULL,R,0);
 lua_pushliteral(L,"version");			/** version */
 lua_pushliteral(L,MYVERSION);
 lua_settable(L,-3);
 lua_pushliteral(L,"__index");
 lua_pushvalue(L,-2);
 lua_settable(L,-3);
 lua_setglobal(L,MYNAME);
 return 1;
}
