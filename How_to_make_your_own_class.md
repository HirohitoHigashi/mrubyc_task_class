# 関数（関数的メソッド）の追加

## 実行部

```
static void c_func1(mrb_vm *vm, mrb_value v[], int argc)
{
  mrbc_printf("func1 called\n");
}
```

## 追加の宣言
rubyから使う関数名（メソッド名）と、実行するC関数を対応づける。
```
mrbc_define_method(0, 0, "func1", c_func1);
```

### mrbc_define_method の引数
```
 mrbc_define_method(struct VM *vm, mrbc_class *cls, const char *name, mrbc_func_t cfunc)
  @param  vm		pointer to vm or 0
  @param  cls		pointer to class or 0.
  @param  name		method name.
  @param  cfunc		pointer to function.
```

## コード例

```
#include "mrubyc.h"

#define MRBC_MEMORY_SIZE (1024*40)
static uint8_t memory_pool[MRBC_MEMORY_SIZE];

extern uint8_t *mrb_file;

int main(void)
{
  mrbc_init(memory_pool, MRBC_MEMORY_SIZE);
  mrbc_define_method(0, 0, "func1", c_func1);

  // load mrb_file

  mrbc_create_task(mrb_file, 0);
  return mrbc_run();
}
```

------------------------------------------------------------------------------
# メソッドへの引数
<dl>
<dt>vm
<dd>VM構造体へのポインタ
<dt>v[]
<dd>引数リスト
<dt>argc
<dd>引数の個数
</dl>

（例）
```ruby
func1( 1, "AA" )
```
```
v[0] = self
v[1] = 1 (Integer)
v[2] = "AA" (String)
argc = 2
```


------------------------------------------------------------------------------
# メソッドの戻り値

返す型によって、簡単にマクロが使える場合と、コードでオブジェクトを作って返す場合がある。

## 整数
```
SET_INT_RETURN(n);
```

## 浮動小数点数
```
SET_FLOAT_RETURN(f);
```

## ブール値
```
SET_TRUE_RETURN();
SET_FALSE_RETURN();
SET_BOOL_RETURN(n);
```

## nil
```
SET_NIL_RETURN();
```

## 文字列

基本形
```
mrbc_value ret = mrbc_string_new(vm, "ABC", 3);
SET_RETURN(ret);
```

C文字列から生成
```
mrbc_string_new_cstr(vm, "ABC");
```

別途 mrbc_alloc したメモリバッファから生成
```
char *buf = mrbc_alloc(vm, 10);
memcpy(buf, "123456789", 10);

// 第3引数は、バッファサイズではなく文字列長を与える
mrbc_value ret = mrbc_string_new_alloc(vm, buf, 9);
```

## 配列

TODO:

------------------------------------------------------------------------------
# クラスの追加

```
mrb_class *my_class = mrbc_define_class(0, "MyClass", 0);
```

### mrbc_define_class の引数
```
mrbc_define_class(struct VM *vm, const char *name, mrbc_class *super)

  @param  vm		pointer to vm or 0.
  @param  name		class name.
  @param  super		super class or 0.
  @return		pointer to defined class.
```

## メソッドの追加
```
mrbc_define_method(0, my_class, "method1", c_myclass_method1);
```

関数の場合と同じ mrbc_define_method を使い、メソッドがどのクラスに属すかを第2引数で指定する。


------------------------------------------------------------------------------
# インスタンス固有のデータを必要とする場合

クラスによっては、生成するインスタンス固有のデータを必要とする場合がある。  
mruby/c では、mrbc_instance 構造体の末尾に可変長データメンバ `data` を確保することで、それをサポートする。

## 方法
new メソッドを用意し、固有データを保存する処理を書く。
```
static void c_myclass_new(mrbc_vm *vm, mrbc_value v[], int argc)
{
  // mrbc_instance 構造体を確保し、v[0] へ代入（=戻り値）する。
  // その際、第3引数で追加データ（この例では int）のサイズを指定する。
  v[0] = mrbc_instance_new(vm, v[0].cls, sizeof(int));

  // instance->data を int へのポインタとみなして、値を代入する。
  *((int *)(v[0].instance->data)) = GET_INT_ARG(1);
}
```

各メソッドで、instance->data から固有データを取り出して使う。
```
static void c_myclass_method1(mrbc_vm *vm, mrbc_value v[], int argc)
{
  // instance->data を int へのポインタと見なして、値を取り出す。
  int n = *((int *)(v[0].instance->data));

  mrbc_printf("specific num = %d\n", n);
}
```

NOTE: 代入、取り出しのマクロが欲しいところ。

### mrbc_value mrbc_instance_new の引数
```
mrbc_instance_new(struct VM *vm, mrbc_class *cls, int size)
  @param  vm    Pointer to VM.
  @param  cls	Pointer to Class (mrbc_class).
  @param  size	size of additional data.
  @return       mrbc_instance object.
```

