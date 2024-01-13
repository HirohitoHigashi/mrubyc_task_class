# mruby/c のスケジューラに追加する Task class の検討用

## 名前

クラス名 `Task`

検討の余地あり。
* prefix をつけるか
* 全く別の名前 （Job とか。ミニコン/汎用機用語かも？）


## 機能（やりたいこと）

1. タスクリストの保持
2. タスクに名前をつけて識別
3. タスクオブジェクトの生成
4. 一時停止（自ら）
5. 一時停止（他タスクから）
6. 再開（他タスクから）
7. 終了（自ら）
8. 終了（他タスクから）
9. 他タスクの終了待ち
10. タスクプライオリティーの変更
11. 実行権を手放す
12. 状態の取得


## 1. タスクリストの保持

Task.list -> Array[Task]  
Task.name_list -> Array["Name"]


## 2. タスクに名前をつけて識別

```
Task.name = "Task1"
```

```
def name=: (String) -> String
def name -> String
```

（検討）setterの戻り値は、void でも良いか？


## 3-1. タスクオブジェクトの生成

```
task1 = Task.new             # 自タスク
task2 = Task.new("TaskName") # その名前を持つタスク
```

### 例外

指定の名前を持ったタスクが存在しない場合、`ArgumentError`

（検討）
Task.new だと、新しいタスクを作るみたい。実際は今動いているタスクを操作するためのオブジェクトを得るという操作。
Task.get とかの方が良いか？

指定のタスク実行完了待ちは、こう？
```
until task1 = Task.get("Task1")
  sleep 1
end
task1.join
```


## 4. 一時停止（自ら）

  def suspend: () -> self

Thread だと、Thread#stop か？

```
Task.suspend
```

## 5. 一時停止（他タスクから）

```
task1.suspend
```

## 6. 再開（他タスクから）

```
task1.resume
```

## 7. 終了（自ら）

  def terminate: () -> self

```
Task.terminate
```

## 8. 終了（他タスクから）

```
task1.terminate
```

## 9. 他タスクの終了待ち

  def join: (?Integer|nil) -> (self | nil)

```
task1.join
```

（検討）
Threadクラスには、引数としてタイムアウト秒を渡せるが、実装が面倒かも？


## 10. タスクプライオリティーの変更
```
task.priority = n    # 0(低) - 255(高)
```

## 11. 実行権を手放す
```
Task.pass
```

## 12. 状態の取得

```
task.status
```

戻り値  
スケジューラの状態を文字列で返せば良いか？

DORMANT
READY
WAITING
SUSPENDED
