//‘1’按钮的弹起事件内容：
btlen input.txt,temp.val
if(temp.val<inputlenth.val)
{
  input.txt+=b1.txt
  show.txt=input.txt
}

//'DEL'按钮的弹起事件内容
input.txt-=1
show.txt=input.txt

//'OK'按钮的弹起事件内容，补充：put_ok=2是我加的自己的代码，用于触发通信协议。不用改。
//���ô�ҳ֮ǰ���ȶԴ�ҳ��loadpageid.val��loadcmpid.val��ֵ�Ϳ����ˣ��������߼���ҳ���Զ�ʵ��
//loadpageid.val��ʾ����ҳ��ҳ��ID,loadcmpid.val��ʾ����ҳ�Ŀؼ�ID
if(p[loadpageid.val].b[loadcmpid.val].type==54)
{
  covx input.txt,p[loadpageid.val].b[loadcmpid.val].val,0,0
}else if(p[loadpageid.val].b[loadcmpid.val].type==59)
{
  covx input.txt,temp.val,0,0
  if(temp.val<0)
  {
    temp.val*=-1
  }
  for(temp2.val=0;temp2.val<p[loadpageid.val].b[loadcmpid.val].vvs1;temp2.val++)
  {
    temp.val*=10
  }
  p[loadpageid.val].b[loadcmpid.val].val=temp.val
  strlen input.txt,temp.val
  temp.val--
  while(temp.val>=0)
  {
    substr input.txt,tempstr.txt,temp.val,1
    if(tempstr.txt==".")
    {
      substr input.txt,tempstr.txt,temp.val+1,p[loadpageid.val].b[loadcmpid.val].vvs1
      covx tempstr.txt,temp2.val,0,0
      strlen tempstr.txt,temp.val
      while(temp.val<p[loadpageid.val].b[loadcmpid.val].vvs1)
      {
        temp2.val*=10
        temp.val++
      }
      p[loadpageid.val].b[loadcmpid.val].val+=temp2.val
      temp.val=-1
    }
    temp.val--
  }
  substr input.txt,tempstr.txt,0,1
  if(tempstr.txt=="-")
  {
    p[loadpageid.val].b[loadcmpid.val].val*=-1
  }
}else
{
  p[loadpageid.val].b[loadcmpid.val].txt=input.txt
}
put_ok=2
page loadpageid.val
