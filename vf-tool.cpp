#include "busybox.hpp"
#include <iostream>
#include <iomanip>
#include "oommf.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "vf.hpp"
#include "filepath.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"

int vf_tool_main(int argc,char **argv){
  if (argc==1){
  std::cout << argv[0] << " file.ovf    para convertir a file.vf" << std::endl;
  std::cout << argv[0] << " file.ovf.gz para convertir a file.vf" << std::endl;
  std::cout << argv[0] << " file.vf     para convertir a file.ovf" << std::endl;
  return 1;
  }
  bool plot=false;
  bool crush=false;
  std::string outpath;
  std::vector<std::string> args;
  for (int idx=1;idx<argc;idx++) {
    if (std::string(argv[idx])==std::string("-png")) {plot=true; idx++; }
    if (std::string(argv[idx])==std::string("-crush")) {crush=true; idx++; }
    if (idx>=argc) break;
    if (std::string(argv[idx])==std::string("-o")) outpath = std::string(argv[++idx]);
    if (idx>=argc) break;
    args.push_back(argv[idx]); 
    }
    if (plot) std::cout << "-png flag detected" << std::endl;
    if (crush) std::cout << "-crush flag detected" << std::endl;
    std::cout << "Output folder:" << outpath << std::endl;
  for (int idx=0;idx<args.size();idx++) {
    rcl::FilePath file(args[idx]);
    std::vector<unsigned char> infile;
    rcl::load_file(infile,args[idx]);
    
    if (file.GetFullExt()==".ovf.gz" || file.GetFullExt()==".ovf"){
      if (file.GetLext()==".gz"){
        std::vector<unsigned char> temp;
        rcl::inflate(temp,infile,true);
        std::swap(infile,temp);
        }
      rcl::ffield vdata;
      std::vector<unsigned char> vfdata;
      rcl::loadOVF2v(vdata,infile);
      vdata.reg_meta("vf_v2","true");
      if (!plot){
      rcl::encode_vf(vfdata,vdata);
      std::string outname = outpath + file.GetRoot() + ".vf";
      std::cout << idx << " : Escribiendo " << outname << std::endl;
      rcl::save_file(outname,vfdata);
        }
      else{
        rcl::normalize(vdata);
        std::vector<unsigned char> pngdata;
        rcl::image32 img = rcl::display_field(vdata,0,"");
        rcl::encode_png(pngdata,img);
        std::string outname = outpath + file.GetRoot() + ".png";
        std::cout << idx << " : Escribiendo " << outname << std::endl;
        rcl::save_file(outname,pngdata); 
        }
      }
    if (file.GetFullExt()==".vf"){
      rcl::ffield vdata;
      rcl::decode_vf(vdata,infile);
      std::vector<unsigned char> ovfdata;
      if (!plot) {
        if (!crush) {
          rcl::saveOVF2vm(ovfdata,vdata);
          std::string outname = outpath + file.GetRoot() + ".ovf";
          std::cout << idx << " : Escribiendo " << outname << std::endl;
          rcl::save_file(outname,ovfdata);
        }
        else{
          if (vdata.get_meta("vf_crush")!="true"){
          
          vdata.reg_meta("vf_crush","true");
          vdata.reg_meta("vf_v2","true");
          int bsize=2147483647;
          int bzl=9;
          int bzs=0;
          int bsaf=0;
          int bsbit=1; 
          int bperm=0;
          for (int zl=1;zl<=9;zl++) 
          for (int zs=0;zs<4;zs++) 
          for (int perm=0;perm<12;perm++)
          for (int saf=0;saf<(1<<vdata.getnodes().z());saf++)
          for (int sbit=0;sbit<2;sbit++) {
            std::vector<unsigned char> vfdata;
            if (perm==6) perm++;
            vdata.reg_meta("vf_zl",rcl::to_str(zl));
            vdata.reg_meta("vf_zs",rcl::to_str(zs));
            vdata.reg_meta("vf_saf",rcl::to_str(saf));
            vdata.reg_meta("vf_perm",rcl::to_str(perm));
            if (sbit==1) vdata.reg_meta("vf_sbit","true");
            else         vdata.reg_meta("vf_sbit","false");
            std::cout << idx << " :" << args[idx] << " : ";
            std::cout <<    "zl=" << zl;
            std::cout << " , zs=" << zs;
            std::cout << " , saf=" << std::setw(4) << std::hex<< saf << std::dec;
            std::cout << " , perm=" << std::setw(2) << perm;
            std::cout << " , span=" << sbit;
            rcl::encode_vf(vfdata,vdata);
            std::cout << " - vsize=" << std::setw(8) << vfdata.size() ;
            std::cout << " - osize=" << std::setw(8) << infile.size() ;
            std::cout << " - bsize=" << std::setw(8) << bsize  ;
            std::cout << "  (" << bzl;
            std::cout << "," << bzs;
            std::cout << "," << std::setw(1) << std::hex<< bsaf << std::dec;
            std::cout << "," << std::setw(2) << bperm;
            std::cout << "," << bsbit << ")";
            if (vfdata.size()<bsize){
              bsize=vfdata.size();
              bzl=zl;
              bzs=zs;
              bsaf=saf;
              bsbit=sbit;
              bperm=perm;
              std::cout << " * ";
              }
            std::cout << std::endl;
          }
        
        vdata.reg_meta("vf_zl",rcl::to_str(bzl));
        vdata.reg_meta("vf_zs",rcl::to_str(bzs));
        vdata.reg_meta("vf_saf",rcl::to_str(bsaf));
        vdata.reg_meta("vf_perm",rcl::to_str(bperm));
        if (bsbit==1) vdata.reg_meta("vf_sbit","true");
        else          vdata.reg_meta("vf_sbit","false");
          
          std::vector<unsigned char> vfdata;
          
            std::cout << std::endl;
            std::cout << "Best: zl=" << bzl;
            std::cout << " , zs=" << bzs;
            std::cout << " , saf=" << std::setw(4) << std::hex<< bsaf << std::dec;
            std::cout << " , perm=" << std::setw(2) << bperm;
            std::cout << " , span=" << bsbit << std::flush; 
            rcl::encode_vf(vfdata,vdata);
            std::cout << " - size=" << std::setw(8) << vfdata.size() << std::endl;
          
          std::string outname = outpath + file.GetRoot() + ".vf";
          std::cout << idx << " : Escribiendo " << outname << " ; ";
          std::cout << infile.size() << " -> " << vfdata.size() << std::endl;
          rcl::save_file(outname,vfdata);
        }
          else std::cout << idx << " : Saltando " << args[idx] << std::endl;

      }
      }
      else{
        rcl::normalize(vdata);
        for (unsigned int layer=0;layer<vdata.getnodes().z();layer++){
          std::vector<unsigned char> pngdata;
          rcl::image32 img = rcl::display_field(vdata,layer,"");
          rcl::encode_png(pngdata,img);
          std::string outname;
          if (vdata.getnodes().z()==1) 
            outname = outpath + file.GetRoot() + ".png";
          else                         
          {
            std::stringstream slayer;
            slayer << layer;
            outname = outpath + file.GetRoot()+ "." + slayer.str() + ".png";
          }
          std::cout << idx << " : Escribiendo " << outname << std::endl;
          rcl::save_file(outname,pngdata); 
        }
      }
    } 
    if (file.GetFullExt()!=".ovf.gz" && file.GetFullExt()!=".ovf" && file.GetFullExt()!=".vf") 
      std::cout << file.GetFullFile() << " no identificado" << std::endl;
  }
  return 0;
}
static rcl::reg_app reg("vf_tool",vf_tool_main);

