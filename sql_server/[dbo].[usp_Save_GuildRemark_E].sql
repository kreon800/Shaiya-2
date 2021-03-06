USE [PS_GameData]
GO
/****** Object:  StoredProcedure [dbo].[usp_Save_GuildRemark_E]    Script Date: 2/1/2021 4:53:27 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER OFF
GO

ALTER Proc [dbo].[usp_Save_GuildRemark_E]

@GuildID int,
@Remark	varchar(64)

AS

SET NOCOUNT ON

IF EXISTS (SELECT GuildID FROM Guilds WHERE GuildID = @GuildID)
BEGIN
	UPDATE GuildDetails SET Remark = REPLACE(@Remark, char(39), char(32)) WHERE GuildID = @GuildID
END

ELSE
BEGIN
	INSERT INTO GuildDetails(GuildID, Remark) VALUES(@GuildID, REPLACE(@Remark, char(39), char(32)))
END

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	RETURN 1
END

ELSE
BEGIN
	RETURN -1
END

SET NOCOUNT OFF
