// ============================================================
// 数字键盘页 —— 加 / 减 功能（两操作数计算器版）
// ------------------------------------------------------------
// 行为定义：
//   左操作数 = bak.txt（按加减键时从 input.txt 搬进来）；bak 为空时
//              退化为控件当前值 p[loadpageid][loadcmpid].val
//   右操作数 = 按下加减键之后输入的 input.txt
//   OK 时分别解析两者再合成，写回控件
//
// 覆盖的用例：
//   原值21 →「+」「9」→ OK           = 30   （主用法）
//   DEL删空 →「5」「0」→ OK          = 50   （纯赋值，不受影响）
//   DEL删空 →「5」「0」「+」「9」→OK = 59   （修复 bug 3）
//   「+」→ DEL                        还原为 21 并退出加减模式（修复 bug 2）
//   「+」→「-」                        只切换运算符，左操作数不丢
// ------------------------------------------------------------
// 需要的东西：
//   程序.s 里加 3 个全局：kb_op / kb_acc / kb_val   （见【0】）
//   键盘页加 1 个控件：bak    文本变量(va, 字符串型, 不可见)
//   键盘页加 1 个控件：opshow 文本(t)  显示运算符，可选，纯视觉反馈
//   原来的 op / acc 数字变量控件可以删掉
// ============================================================


// ------------------------------------------------------------
// 【0】程序.s —— 追加三行全局变量声明
// ------------------------------------------------------------
int kb_op=0
int kb_acc=0
int kb_val=0


// ------------------------------------------------------------
// 【1】键盘页「前初始化事件」—— 追加，不要删原有内容
// ------------------------------------------------------------
kb_op=0
bak.txt=""
opshow.txt=""


// ------------------------------------------------------------
// 【2】「加」按钮 → 弹起事件
//      input 非空才搬去 bak，这样连按 加/减 只是切运算符，不丢左操作数
// ------------------------------------------------------------
strlen input.txt,temp.val
if(temp.val>0)
{
  bak.txt=input.txt
  input.txt=""
  show.txt=""
}
kb_op=1
opshow.txt="+"


// ------------------------------------------------------------
// 【3】「减」按钮 → 弹起事件
//      注意：这不是原来输负数用的那个 '-' 键，那个键保持原样别动
// ------------------------------------------------------------
strlen input.txt,temp.val
if(temp.val>0)
{
  bak.txt=input.txt
  input.txt=""
  show.txt=""
}
kb_op=2
opshow.txt="-"


// ------------------------------------------------------------
// 【4】DEL 按钮 → 弹起事件（整段替换原有两行）
//      删空后若处于加减模式，则还原左操作数并退出加减模式
// ------------------------------------------------------------
input.txt-=1
show.txt=input.txt
strlen input.txt,temp.val
if(temp.val==0)
{
  if(kb_op!=0)
  {
    input.txt=bak.txt
    show.txt=input.txt
    bak.txt=""
    kb_op=0
    opshow.txt=""
  }
}


// ------------------------------------------------------------
// 【5】新增隐藏按钮 bparse → 「按下事件」
//      当子程序用：把 input.txt 解析成 xfloat 的整数值存进 kb_acc
//      （即原 OK 里 type==59 的那段解析，结果改存 kb_acc 而非直接写控件）
//      这个按钮不需要被人点到，摆到屏幕外或盖在别的控件下面都行
// ------------------------------------------------------------
kb_acc=0
strlen input.txt,temp.val
if(temp.val>0)
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
  kb_acc=temp.val
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
      kb_acc+=temp2.val
      temp.val=-1
    }
    temp.val--
  }
  substr input.txt,tempstr.txt,0,1
  if(tempstr.txt=="-")
  {
    kb_acc*=-1
  }
}


// ------------------------------------------------------------
// 【6】OK 按钮 → 弹起事件（整段替换原有内容）
// ------------------------------------------------------------
if(p[loadpageid.val].b[loadcmpid.val].type==54)
{
  // 右操作数
  kb_acc=0
  strlen input.txt,temp.val
  if(temp.val>0)
  {
    covx input.txt,temp.val,0,0
    kb_acc=temp.val
  }
  if(kb_op!=0)
  {
    // 左操作数：bak 有内容用它，否则用控件当前值
    strlen bak.txt,temp.val
    if(temp.val>0)
    {
      covx bak.txt,temp.val,0,0
      kb_val=temp.val
    }else
    {
      kb_val=p[loadpageid.val].b[loadcmpid.val].val
    }
    if(kb_op==2)
    {
      kb_acc*=-1
    }
    kb_acc+=kb_val
  }
  p[loadpageid.val].b[loadcmpid.val].val=kb_acc
}else if(p[loadpageid.val].b[loadcmpid.val].type==59)
{
  click bparse,1          // 解析右操作数 → kb_acc
  kb_val=kb_acc           // 暂存右操作数
  if(kb_op!=0)
  {
    strlen bak.txt,temp.val
    if(temp.val>0)
    {
      input.txt=bak.txt
      click bparse,1      // 解析左操作数 → kb_acc
    }else
    {
      kb_acc=p[loadpageid.val].b[loadcmpid.val].val
    }
    if(kb_op==2)
    {
      kb_val*=-1
    }
    kb_acc+=kb_val        // kb_acc=左, kb_val=右(减法已取反)
  }
  p[loadpageid.val].b[loadcmpid.val].val=kb_acc
}else
{
  p[loadpageid.val].b[loadcmpid.val].txt=input.txt
}
put_ok=2
page loadpageid.val
