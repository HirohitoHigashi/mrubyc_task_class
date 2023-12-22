# mruby/c のスケジューラに追加する Task class の検討用

## 名前

クラス名 `Task`

検討の余地あり。
* prefix をつけるか
* 全く別の名前 （Job とか。ミニコン/汎用機用語かも？）


## 機能（やりたいこと）

1. タスクリストの保持
2. タスクに名前をつけて識別
3. 一時停止（自ら）
4. 一時停止（他タスクから）
5. 再開（他タスクから）
6. 終了（自ら）
7. 終了（他タスクから）
8. 他タスクの終了待ち


## 1. タスクリストの保持

Task.list -> Array[Task]  
Task.list -> Array["Name"]  名前の一覧が良いのでは？

## 2. タスクに名前をつけて識別

  def name=: (String) -> String

  def name -> String


## 3. 一時停止（自ら）

  def suspend: () -> self


## 4. 一時停止（他タスクから）

## 5. 再開（他タスクから）

resume

## 6. 終了（自ら）

  def terminate: () -> self


## 7. 終了（他タスクから）

## 8. 他タスクの終了待ち

  def join: (?Integer|nil) -> (self | nil)

