﻿<%@ Page Language="C#" Inherits="System.Web.Mvc.ViewPage" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title></title>
</head>
<body>
    <div>
    <%using(Html.BeginForm("AlterAction","News",FormMethod.Post,null)){ %>
    <div align="left">新闻标题<%=Html.TextBox("topic",ViewData["NewsTopic"])%></div>
    <%=Html.Hidden("NewsId",ViewData["NewsId"],null) %>
    <div align="left"><div align="left">新闻内容</div><div><%=Html.TextArea("contents", ViewData["contents"] as string, 30, 100, null)%></div></div>
    <input type="submit" value="更新" />
    <%} %>
    </div>
</body>
</html>
