# Game Rom Mutator GUI by W R "Ron" Spain Jr

import os
import re
import sys
import tkinter as tk
from tkinter.filedialog import askopenfilename


def go():
 n=num.get()
 if n=="" or int(num.get())<1:
  tk.messagebox.showinfo("Warning!","No roms to be generated!")
  return

 s="./mutate "+rom.get()

 s+=" -n "+str(num.get())

 s+=" -v "+str(verb.get())

 n=rate.get()
 if n!="":
  n=float(n)
  if n>=1:
   s+=" -c "+str(n)
  else:
   s+=" -r "+str(n)

 v=begin.get()
 if v!="":
  if v.find('%'):
   s+=" -B "+v.replace("%","")
  else:
   s+=" -b "+v

 v=end.get()
 if v!="":
  if v.find('%'):
   s+=" -E "+v.replace("%","")
  else:
   s+=" -e "+v

 r=os.popen(s).read()
 if r:
  tl=tk.Toplevel()
  tl.title("Results")
  txt=tk.Label(master=tl,text=r)
  txt.pack()
  bye=tk.Button(master=tl,text="Close",command=tl.destroy,border=5)
  bye.pack()



win=tk.Tk()
win.title("Game Rom Mutator")
win.resizable(width=False,height=False)

rompath=askopenfilename(
 filetypes=[
	("Common","*.nes *.gb *.sms *.gen *.smc *.sfc *.a26"),
	("All Files", "*.*")
 ]
)

if not rompath:
 sys.exit();

frm=tk.Frame(master=win)


rom=tk.Entry(master=frm,width=32)
rate=tk.Entry(master=frm,width=8)
begin=tk.Entry(master=frm,width=8)
end=tk.Entry(master=frm,width=8)
num=tk.Entry(master=frm,width=8)

verb=tk.IntVar()
c1=tk.Checkbutton(master=frm,text="Verbose",variable=verb)

go=tk.Button(master=frm,text="GO!",command=go,border=5)

r=re.search("\.(zip|7z|gz|gzip|xz|rar|lz|lzma)$",rompath)
if r:
 tk.messagebox.showinfo("Warning!","It appears you might be trying to mutate a compressed archive! You should really uncompress your rom and restart.")

stats=os.stat(rompath)

l1=tk.Label(master=frm,text="ROM:")
l2=tk.Label(master=frm,text="Rate:")
l3=tk.Label(master=frm,text="Start at:")
l4=tk.Label(master=frm,text="End at:")
l5=tk.Label(master=frm,text="Create:")
l6=tk.Label(master=frm,text=str(stats.st_size)+" bytes")


frm.grid(row=0,column=0,padx=8,pady=8)
l1.grid(row=0,column=0)
rom.grid(row=0,column=1)
l6.grid(row=0,column=2)

l2.grid(row=1,column=0)
rate.grid(row=1,column=1)

l3.grid(row=2,column=0)
begin.grid(row=2,column=1)

l4.grid(row=3,column=0)
end.grid(row=3,column=1)

l5.grid(row=4,column=0)
num.grid(row=4,column=1)

c1.grid(row=5,column=1)
go.grid(row=5,column=2)


rom.insert(0,rompath);
rate.insert(0,"5.0");
begin.insert(0,"1%");
end.insert(0,"99%");
num.insert(0,"8");
c1.select();

win.mainloop()


