//
// 本程序演示文件操作类CFile，打开文件并写入数据，采用了临时文件缓存的方法
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo8 xmlfilename\n\n");

    printf("Example:./demo8 demo8.xml\n\n");

    printf("本程序演示文件操作类CFile，打开文件并写入数据，采用了临时文件缓存的方法。\n");

    return -1;
  }

  CFile File;

  // 打开文件
  if (File.OpenForRename(argv[1],"w")==FALSE)
  {
    printf("File.OpenForRename(%s) failed.\n",argv[1]); return -1;
  }
  
  // 往文件写入内容
  File.Fprintf(\
     "<?xml version='1.0' encoding='gbk'?>\n"\
     "<data>\n"\
     "<id>1001</id><name>张一</name><sex>男</sex><age>21</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1002</id><name>张二</name><sex>男</sex><age>22</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1003</id><name>张三</name><sex>男</sex><age>23</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1004</id><name>张四</name><sex>男</sex><age>24</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1005</id><name>张五</name><sex>男</sex><age>25</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1006</id><name>张六</name><sex>男</sex><age>26</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1007</id><name>张七</name><sex>男</sex><age>27</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "<id>1008</id><name>张八</name><sex>女</sex><age>28</age><memo>备注：\n"\
     "1、出生在四川的一个农村；\n"\
     "2、在深圳长大的；\n"\
     "3、长大了在广州工作。</memo><endl/>\n"\
     "<id>1009</id><name>张九</name><sex>男</sex><age>29</age><memo>备注信息，没有换行。</memo><endl/>\n"\
     "</data>\n");

  // 等待30秒，可以在目录中看到demo8.xml.tmp的临时文件
  sleep(30);

  // 关闭文件
  File.CloseAndRename();

  return 0;
}

