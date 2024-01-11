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

## 5. 一時停止（他タスクから）

## 6. 再開（他タスクから）

resume

## 7. 終了（自ら）

  def terminate: () -> self


## 8. 終了（他タスクから）

## 9. 他タスクの終了待ち

  def join: (?Integer|nil) -> (self | nil)


## 10. タスクプライオリティーの変更
```
task.priority = n
```

## 11. 実行権を手放す
```
Task.pass
```
