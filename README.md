# Alista OS

Alista OS是一款基于x86的玩具内核。它非常简单，实现参考了xv6。可以说是xv6的加强版。

目录：

1. [Environment]()
2. [Installation]()
3. [Usage]()
4. [Contribution]()
5. [License]()
6. [Project status]()
7. [Code style]()
8. [Contact]()

## Environment

待项目完成后补充

## Installation

待项目完成后补充

## Usage

待项目完成后补充

## Contribution
欢迎 Pull Request。对于较大的改动，请先打开一个 issue，谈一下您想要更改的内容。提交前先进行足够的测试。

## License

Alista OS使用GPL v3.0的条款。有关完整的许可信息，请参阅此存储库中的[LICENSE](https://github.com/astzls213/AlistaOS/blob/master/LICENSE)文件。

## Project status

Alista OS仍在施工中。

## Code style

如果想要对Alista进行修改，请遵循以下代码风格：

```c
// 当使用if switch while...时请这样：
if (x) {
  
}
// 而不是
if(x){
  
}

// 当定义函数时，请这样，这样使得可以grep ^function
type
function(arg1)
{
  
}
// 而不是
type function(arg1) {
  
}

// 调用函数无需空格，即
somefunc(arg1,arg2);

// 函数名用_隔开，例如：
type
something_else_func()
{
}
// 而不是somethingElseFunc()
```

## Contact

作者来自暨南大学JNU，下面有联系方式哦！

- Email: astzls213@163.com
- Github: https://github.com/astzls213